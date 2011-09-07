/*
 * filetree.h
 *
 *  Created on: 2011-4-28
 *      Author: FanOpi
 */

#ifndef FILETREE_H_
#define FILETREE_H_

#include "../../include/support/runnable.h"
#include "../../include/support/datastruct/tree.hpp"
#include "../../include/support/datastruct/map.hpp"
#include "../../include/support/datastruct/vector.hpp"
#include "../serialize/xml.h"

namespace YF{

// 利用filetree来封装xmldocument
class FileTree : public XmlDocument{
 public:
	FileTree(long id){
		tree_id_ = id;
		my_trees_.Insert(tree_id_,this);
	}
	FileTree(const YFCHAR* xmlName, long id) : XmlDocument(xmlName) {
		tree_id_ = id;
		my_trees_.Insert(tree_id_,this);
	}

	XmlNode* FindXmlElement(long father_folder_id);
	long AddFolder(const YFCHAR* folder_name); // under the root node
	long AddFolder(long father_folder_id, const YFCHAR* folder_name);
	long AddFile(const YFCHAR* file_name); // under the root node
	long AddFile(long father_folder_id, const YFCHAR* file_name);

 private:
	long tree_id_;
	Map<long, FileTree*> my_trees_;
};

class TreeBuilder : public Runnable{
 public:
	static TreeBuilder* GetTreeBuilder();
	virtual int Task(long paramter);
	virtual bool Run();
	FUNSTATE Report(OperatorCode  operator_code);
	FUNSTATE SendMessage(Thread_type who, OperatorCode  operator_code);
	FUNSTATE  InMessageQueue(MessageObj* p_message);
	MessageObj* GetMessageFormQueue();

 private:
	static long tree_id_;
	long curr_tree_id;
	Map<long, FileTree*> my_trees_;
	static TreeBuilder* singleton_;
	TreeBuilder(){
		running_ = false;
	}
	YFThread thread_;
	MessageQueue message_queue_;
	bool running_;
};

}/// namespace YF
#endif /* FILETREE_H_ */
