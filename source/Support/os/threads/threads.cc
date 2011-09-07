/*
 * threads.cc
 *
 *  Created on: 2011-4-26
 *      Author: FanOpi
 */

#include "threads.hpp"
namespace YF{

int  YFThread::ThreadProc(ThreadInfo* p_object){
	 if(NULL != p_object)
		return (p_object->obj->*(p_object->threadFunc))(p_object->param);
	 return 0;
}

} /// namespace YF
