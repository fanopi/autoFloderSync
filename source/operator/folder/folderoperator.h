/*
 * folderOp.h
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_OPERATOR_FLODER_FOLDEROP_H_
#define FPJ_SOURCE_OPERATOR_FLODER_FOLDEROP_H_

#include "../../include/typedef.h"
#include "../../include/os/file/file.h"
#include "../../include/support/datastruct/queue.hpp"
#include "../fileoperator.h"

namespace YF{

class FileScannor{
 public:
	static FileScannor* GetFileScannor();
	FUNSTATE Scan(long string_index, long recruse_scan);
	FUNSTATE ReturnScanResult();
 private:
	FileScannor(){
		scan_file_number = 0;
	}
	YFCHAR* GetFileName(FileFindData* p_find_data) const;
	bool IsFolder(FileFindData* p_find_data) const;
	FUNSTATE ScanFolder(const YFCHAR* p_file_name);
	int scan_file_number;
	struct FileDescript{
		long string_index;
		int		file_type;

		void operator =(const FileDescript& fileDes){			
			this->file_type = fileDes.file_type;
			this->string_index = fileDes.string_index;			
		}
	};
	enum{
		FILE = 0,
		FOLDER = 1,
	};
	Queue<FileDescript> scan_result_queue_;
	Queue<long> scan_help_queue_;
	static FileScannor* singleton_;
};

class FloderOperator{
 public:
		FUNSTATE ReturnScanResult();
		FUNSTATE FloderOperator::SendMessage(Thread_type who,
		                                     OperatorCode  operator_code,
		                                     long param1 = 0,
		                                     long param2 = 0,
		                                     long param3 =0,
		                                     long param4 = 0);
		FUNSTATE Report(OperatorCode  operator_code);
		static FloderOperator* GetFloderOperator();
		FUNSTATE  InMessageQueue(MessageObj* p_message);
		MessageObj* GetMessageFormQueue();
		virtual int Task(long paramter);
		virtual bool Run();
 private:
		FloderOperator(){
			running_ = false;
		}
		static FloderOperator* singleton_;
		YFThread thread_;
		MessageQueue message_queue_;
		bool running_;
};

} /// namespace YF
#endif /* FPJ_SOURCE_OPERATOR_FLODER_FOLDEROP_H_ */
