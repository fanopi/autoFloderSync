/*
 * file_operation.h
 *
 *  Created on: 2011-4-15
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_FILEOPERATOR_FILE_OPERATION_H_
#define FPJ_SOURCE_INCLUDE_FILEOPERATOR_FILE_OPERATION_H_

#include "../../typedef.h"

namespace YF{

typedef void* FileHandle;
typedef struct _FileFindData{
	YFWORD 			file_attributes; //文件属性
	YFFILETIME 	creation_time; // 文件创建时间
	YFFILETIME 	last_access_time; // 文件最后一次访问时间
	YFFILETIME 	last_write_time; // 文件最后一次修改时间
	YFDWORD 		file_size_high; // 文件长度高32位
	YFDWORD 		file_size_low; // 文件长度低32位
	YFDWORD 		reserved0; // 系统保留
	YFDWORD 		reserved1; // 系统保留
	YFCHAR 			file_name[512]; // 长文件名
	YFCHAR 			alternate_file_name[ 14 ]; // 8.3格式文件名
} FileFindData;

enum {
	YF_INVALID_HANDLE_VALUE,

};

class FileOperation{
 public:
	virtual FileHandle FindFirst(const YFCHAR* p_scan_Path, FileFindData* p_file_find_data)=0;
	virtual FUNSTATE FindNext(FileHandle file_handle, FileFindData* p_file_find_data)=0;
	virtual FUNSTATE FindClose(FileHandle file_handle)=0;
};

} /// namespace YF ///
#endif /* FPJ_SOURCE_INCLUDE_FILEOPERATOR_FILE_OPERATION_H_ */
