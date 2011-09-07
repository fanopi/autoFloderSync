/*
 * fileOperator.h
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_OPERATOR_FILEOPERATOR_H_
#define FPJ_SOURCE_OPERATOR_FILEOPERATOR_H_

#include "../include/support/runnable.h"

namespace YF{

class FileOperator : public Runnable{
 public:
	virtual FUNSTATE  InMessageQueue(MessageObj* p_message)=0;
	virtual MessageObj* GetMessageFormQueue()=0;
	virtual bool Run()=0;
	virtual int Task(long paramter) = 0;
};

} /// namespace YF
#endif /* FPJ_SOURCE_OPERATOR_FILEOPERATOR_H_ */
