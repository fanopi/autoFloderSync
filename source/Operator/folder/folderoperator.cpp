/*
 * folderOp.cpp
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#include "folderoperator.h"
#include "../operatorcode.h"
#include "../../tablemanager/smallobject/sharedstring.h"
#include "../../include/os/file/file.h"
#include "../../include/os/file/windows/file.h"
#include "../filetyperegistor.h"
#include "../../include/library/string.h"
#include "../../govern/govern.h"
#include "../../include/library/stdio.h"

namespace YF{
FileScannor* FileScannor::singleton_ = NULL;
FileScannor* FileScannor::GetFileScannor(){
	if (NULL == singleton_)
		singleton_ = new FileScannor();
	return singleton_;
}

YFCHAR* FileScannor::GetFileName(FileFindData* p_find_data) const {
 if (NULL != p_find_data)
	 return p_find_data->file_name;
}

bool FileScannor::IsFolder(FileFindData* p_find_data) const {
	return FILE_ATTRIBUTE_DIRECTORY & p_find_data->file_attributes;
}

// NOTED: Only the Breadth-First-Search is allowed.
FUNSTATE FileScannor::ScanFolder(const YFCHAR* p_file_name){
	FileHandle fileHandle;
	FileFindData	findData;
	YFCHAR p_new_file_name[512];
	YFCHAR p_scan_file_name[512];
	FUNSTATE ret = RTN_OK;
	SharedString* p_string_build = SharedString::GetSharedString();
	FileOperation* p_win_file_op = new WinFileOperation;
	if( NULL == p_file_name)
		goto  ERROR_EXIT;
	StrcpyUni(p_scan_file_name,p_file_name);
	StrcatUni(p_scan_file_name, "\\*.*");

	for (fileHandle = p_win_file_op->FindFirst(p_scan_file_name, &findData);
			INVALID_HANDLE_VALUE != fileHandle && RTN_OK == ret;
			ret = p_win_file_op->FindNext(fileHandle, &findData)){

		YFCHAR*	p_find_file_name = this->GetFileName(&findData);

		const YFCHAR* p_flited_char1 = ".";
		const YFCHAR* p_flited_char2 = "..";
		if ( 0 == StrcmpUni(p_find_file_name, p_flited_char1)
			|| 0 == StrcmpUni(p_find_file_name, p_flited_char2) )
			continue;

		StrcpyUni(p_new_file_name, p_file_name);
		StrcatUni(p_new_file_name, "\\");
		StrcatUni(p_new_file_name, p_find_file_name);
		StringTrait trait = p_string_build->StoreString(p_new_file_name);
		long string_index = p_string_build->GetStringIndex(trait);
		FileDescript fileDesc;
		fileDesc.string_index = string_index;
		fileDesc.file_type = FILE;

		if (IsFolder(&findData)){
			scan_help_queue_.InQueue(string_index);
			fileDesc.file_type = FOLDER;
		}

		scan_result_queue_.InQueue(fileDesc);
		scan_file_number++;
	}/// for
	p_win_file_op->FindClose(fileHandle);

	return RTN_OK;
ERROR_EXIT:
	return RTN_FAIL;
}

FUNSTATE FileScannor::Scan(long string_index, long recruse_scan){
	FUNSTATE ret = RTN_OK;
	SharedString* p_string_build = SharedString::GetSharedString();
	ret = ScanFolder(p_string_build->GetString(string_index));
	while(recruse_scan && !scan_help_queue_.IsQueueEmpty()){
		string_index = scan_help_queue_.OutQueue();
		ScanFolder(p_string_build->GetString(string_index));
	}
	return ret;
}

FUNSTATE FileScannor::ReturnScanResult(){
	FUNSTATE ret = RTN_OK;
	FloderOperator* p_folder = FloderOperator::GetFloderOperator();
	while(!scan_result_queue_.IsQueueEmpty()){
		long file_type = 0;
		FileDescript fileDes = scan_result_queue_.OutQueue();
		if (fileDes.file_type == FOLDER)
			file_type = 1;
		p_folder->SendMessage(TREEBUILDER, OP_CODE_INSERT_NODE, 0, 0, file_type, fileDes.string_index);
	}
	return ret;
}

//////////   FloderOperator   /////////////////
FloderOperator* FloderOperator::singleton_ = NULL;
FloderOperator* FloderOperator::GetFloderOperator(){
	if (NULL == singleton_)
		singleton_ = new FloderOperator;
	return singleton_;
}

FUNSTATE  FloderOperator::InMessageQueue(MessageObj* p_message){
	return this->message_queue_.InMessageQueue(p_message);
}

MessageObj* FloderOperator::GetMessageFormQueue(){
	return this->message_queue_.GetMessageFormQueue();
}

FUNSTATE FloderOperator::Report(OperatorCode  operator_code){
	return SendMessage(GOVERNOR, operator_code);
}

FUNSTATE FloderOperator::SendMessage(Thread_type who,
                                     OperatorCode  operator_code,
                                     long param1,
                                     long param2,
                                     long param3,
                                     long param4){
	FUNSTATE state = RTN_FAIL;
	MessageObj* p_message = new MessageObj(operator_code, who);
	if (NULL == p_message)
		goto EXIT;

	Governor* pGovernor = Governor::GetGovernor();
	if (NULL == pGovernor)
		goto EXIT;	
	p_message->SetParamter(param1,param2,param3,param4);
	state = pGovernor->InMessageQueue(p_message);

	state = RTN_OK;
EXIT:
	return state;
}

int FloderOperator::Task(long paramter){
	OperatorCode  operator_code;
	MessageObj* 	p_new_message 	= NULL;

  while(true){
		p_new_message = this->GetMessageFormQueue();
		operator_code = p_new_message->GetOpCode();

		switch (operator_code){
			case OP_CODE_FOLDER_ENUM:{
				FileScannor* p_file_scannor = FileScannor::GetFileScannor();
				long string_index;
				long recruse_scan;
				p_new_message->GetParamter(&string_index,&recruse_scan);
				p_file_scannor->Scan(string_index,recruse_scan);
				Report(OP_CODE_FOLDER_ENUM_DONE);
				break;
			}

			case OP_CODE_FOLDER_REPORT:{
				Report(OP_CODE_FOLDER_ENUM_DOING);
				break;
			}

			case OP_CODE_FOLDER_RETURN_RESULT:{
				FileScannor* p_file_scannor = FileScannor::GetFileScannor();
				p_file_scannor->ReturnScanResult();
				SendMessage(TREEBUILDER, OP_CODE_OUTPUT);
				break;
			}

			default:
				break;
		}
  }
}

bool FloderOperator::Run(){
	if (false == running_){
		running_ = true;
		return thread_.Start(this, (ThreadFunction)&FloderOperator::Task, 0);
	}else
	{
		return false;
	}
}


} /// namespace YF
