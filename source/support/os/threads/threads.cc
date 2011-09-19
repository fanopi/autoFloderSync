/*
 * threads.cc
 *
 *  Created on: 2011-4-26
 *      Author: FanOpi
 */

#include "threads.hpp"

namespace yf{

int  YFThread::ThreadProc(NullThreadInfo* object){
	 if(NULL != object)
		return (object->object->*(object->function))(object->parameter);
	 return 0;
}

} /// namespace YF
