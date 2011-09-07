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
	YFWORD 			file_attributes; //�ļ�����
	YFFILETIME 	creation_time; // �ļ�����ʱ��
	YFFILETIME 	last_access_time; // �ļ����һ�η���ʱ��
	YFFILETIME 	last_write_time; // �ļ����һ���޸�ʱ��
	YFDWORD 		file_size_high; // �ļ����ȸ�32λ
	YFDWORD 		file_size_low; // �ļ����ȵ�32λ
	YFDWORD 		reserved0; // ϵͳ����
	YFDWORD 		reserved1; // ϵͳ����
	YFCHAR 			file_name[512]; // ���ļ���
	YFCHAR 			alternate_file_name[ 14 ]; // 8.3��ʽ�ļ���
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
