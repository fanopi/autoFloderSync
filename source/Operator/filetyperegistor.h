/*
 * registor.h
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_OPERATOR_FILE_REGISTOR_H_
#define FPJ_SOURCE_OPERATOR_FILE_REGISTOR_H_

#include "../include/typedef.h"
#include "fileoperator.h"
namespace YF{

class FileRegister{
 public:
	FileRegister();
	FileOperator* RegisterFile(long file_name_string_index);
};

} /// namespace YF
#endif /* FPJ_SOURCE_OPERATOR_FILE_REGISTOR_H_ */
