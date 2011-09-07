/*
 * filetree.cc
 *
 *  Created on: 2011-4-28
 *      Author: FanOpi
 */

#include "filetreebuilder.h"

#include "../../operator/operatorcode.h"
#include "../../govern/govern.h"
#include "../smallobject/sharedstring.h"

namespace YF{

XmlNode* FileTree::FindXmlElement(long father_folder_id){
	if (father_folder_id == tree_id_){
		return (XmlNode*)this;
	}
}

long FileTree::AddFolder(const YFCHAR* folder_name){
	return AddFolder(tree_id_, folder_name);
}

long FileTree::AddFolder(long father_folder_id, const YFCHAR* folder_name){
	if (NULL == folder_name)
		return NULL;
	XmlElement* p_xmlElement = new XmlElement(folder_name);
	XmlNode* p_father_XmlNode = FindXmlElement(father_folder_id);
	p_father_XmlNode->LinkEndChild(p_xmlElement);
	return NULL;
}

long FileTree::AddFile(const YFCHAR* file_name){
	return AddFile(tree_id_, file_name);
}

long FileTree::AddFile(long father_folder_id, const YFCHAR* file_name){
	if (NULL == file_name)
		return NULL;
	XmlElement* p_xmlElement = new XmlElement(file_name);
	XmlNode* p_father_XmlNode = FindXmlElement(father_folder_id);
	p_father_XmlNode->LinkEndChild(p_xmlElement);
	return NULL;
}


///////////////// class TreeBuilder ///////////////////////////////////////////
long TreeBuilder::tree_id_ = 1;
TreeBuilder* TreeBuilder::singleton_ = NULL;
TreeBuilder* TreeBuilder::GetTreeBuilder(){
	if (NULL == singleton_)
		singleton_ = new TreeBuilder;
	return singleton_;
}

FUNSTATE  TreeBuilder::InMessageQueue(MessageObj* p_message){
	return this->message_queue_.InMessageQueue(p_message);
}

MessageObj* TreeBuilder::GetMessageFormQueue(){
	return this->message_queue_.GetMessageFormQueue();
}

FUNSTATE TreeBuilder::Report(OperatorCode  operator_code){
	return SendMessage(GOVERNOR, operator_code);
}

FUNSTATE TreeBuilder::SendMessage(Thread_type who, OperatorCode  operator_code){
	FUNSTATE state = RTN_FAIL;
	MessageObj* p_message = new MessageObj(operator_code, who);
	if (NULL == p_message)
		goto EXIT;

	Governor* pGovernor = Governor::GetGovernor();
	if (NULL == pGovernor)
		goto EXIT;
	state = pGovernor->InMessageQueue(p_message);

	state = RTN_OK;
EXIT:
	return state;
}


int TreeBuilder::Task(long paramter){
	MessageObj* 	p_new_message  = NULL;
	SharedString* p_string_build = SharedString::GetSharedString();

  while(true){
		p_new_message = this->GetMessageFormQueue();
		if (NULL == p_new_message)
			continue;

		switch (p_new_message->GetOpCode()){
			case OP_CODE_INSERT_NODE:{
				long tree_id; // get the tree id form the message param1
				long father_id; // get the father node id form the message param2
				long node_type; // get the inserting node type form them message param3
				long node_string_index; // get the inserting node string index form them message param4
				p_new_message->GetParamter(&tree_id, &father_id, &node_type, &node_string_index);
				switch(node_type){
					case 1 /*FOLDER*/:{
						FileTree* p_fileTree = (*(my_trees_.Find(curr_tree_id))).second;
						p_fileTree->AddFolder(p_string_build->GetString(node_string_index));
						break;
					}

					case 0 /*FILE*/:{
						FileTree* p_fileTree = (*(my_trees_.Find(curr_tree_id))).second;
						p_fileTree->AddFile(p_string_build->GetString(node_string_index));
						break;
					}
				}
				break;
			}

			case OP_CODE_OUTPUT:{
				FileTree* p_fileTree = (*(my_trees_.Find(curr_tree_id))).second;
				p_fileTree->SaveFile("c:\\output.xml");
				break;
			}

			case OP_CODE_BUILD_TREE:{
				FileTree* p_fileTree = new FileTree(tree_id_);
				my_trees_.Insert(tree_id_, p_fileTree);
				curr_tree_id = tree_id_;
				tree_id_++;
				// report to governor the tree id.
				Report(OP_CODE_BUILD_TREE_DONE);
				break;
			}

			case OP_CODE_COMPAIR_TREE:{

				break;
			}

			default:
				break;
		}

  }
}

bool TreeBuilder::Run(){
	if (false == running_){
		running_ = true;
		return thread_.Start(this, (ThreadFunction)&TreeBuilder::Task, 0);
	}else{
		return false;
	}
}

}/// namespace YF
