/*
 * fileOperation.h
 *
 *  Created on: 2011-4-9
 *      Author: Administrator
 */

#ifndef FPJ_SOURCE_INCLUDE_FILEOPERATOR_WINDOWS_FILEOPERATION_H_
#define FPJ_SOURCE_INCLUDE_FILEOPERATOR_WINDOWS_FILEOPERATION_H_

#include "../file.h"

namespace YF{

class WinFileOperation : public FileOperation{
 public:
	FileHandle FindFirst(const YFCHAR* p_scan_Path, FileFindData* p_file_find_data);
	FUNSTATE FindNext(FileHandle file_handle, FileFindData* p_file_find_data);
	FUNSTATE FindClose(FileHandle file_handle);
};

} /// namespace YF
#endif /* FPJ_SOURCE_INCLUDE_FILEOPERATOR_WINDOWS_FILEOPERATION_H_ */
