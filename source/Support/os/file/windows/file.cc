/*
 * fileOperation.cpp
 *
 *  Created on: 2011-4-9
 *      Author: Administrator
 */

#include "file.h"
#include <Windows.h>

namespace YF{

FileHandle WinFileOperation::FindFirst(const YFCHAR* p_scan_Path, FileFindData* p_file_find_data){
	return ::FindFirstFile(p_scan_Path, (LPWIN32_FIND_DATAA)p_file_find_data);
}

FUNSTATE WinFileOperation::FindNext(FileHandle file_handle, FileFindData* p_file_find_data){
	if (true == ::FindNextFile(file_handle, (LPWIN32_FIND_DATAA)p_file_find_data))
		return RTN_OK;
	else
		return RTN_FAIL;
}

FUNSTATE WinFileOperation::FindClose(FileHandle file_handle){
	if (true == ::FindClose(file_handle))
		return RTN_OK;
	else
		return RTN_FAIL;
}

static void UnitTest(const YFCHAR* p_file_name){
	FileHandle file_handle;
	FUNSTATE ret = RTN_OK;
	FileFindData	findData;
	WinFileOperation* p_win_file_op = new WinFileOperation;

	for (file_handle = p_win_file_op->FindFirst(p_file_name, &findData);
			INVALID_HANDLE_VALUE != file_handle && RTN_OK == ret;
			ret = p_win_file_op->FindNext(file_handle, &findData) ){

	}/// for (FileHandle fileHandle = p_w ........

	p_win_file_op->FindClose(file_handle);

}

} /// namespace YF
