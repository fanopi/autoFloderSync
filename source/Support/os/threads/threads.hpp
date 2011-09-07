/*
 * threadsPart.h
 *
 *  Created on: 2011-4-20
 *      Author: FanOpi
 */

#ifndef THREADSPART_H_
#define THREADSPART_H_

#ifdef _WIN32
#include "windows/threads.h"
#endif

#ifdef _LINUX

#endif

namespace YF{

template <typename T>
struct _ThreadInfo{
	typedef int (T::*ThreadFun)(long);
	T* 				obj;
	long 			param;
	ThreadFun threadFunc;
};

class NULLCLASS{ };

typedef _ThreadInfo<NULLCLASS> ThreadInfo;
typedef _ThreadInfo<NULLCLASS>::ThreadFun ThreadFunction;


class YFThread{
 public:
		YFThread(){
		handle_ = (HANDLE)0;
	 };
	 ~YFThread(){};

template<typename ObjType>
 bool Start(ObjType* p_obj,ThreadFunction func,long param = 0);

private:
 static int  ThreadProc(ThreadInfo* p_object);

private:
 YFHANDLE 	  handle_;
 ThreadInfo*  p_info_;
};


template<typename ObjType>
bool YFThread::Start(ObjType* p_obj,ThreadFunction func, long param){
 if(handle_ || NULL == p_obj || NULL == func)
  return false; // thread have running.

 _ThreadInfo<ObjType>* p_threadInfo = new _ThreadInfo<ObjType>;
 p_threadInfo->obj = p_obj;
 p_threadInfo->param = param;
 p_threadInfo->threadFunc = (_ThreadInfo<ObjType>::ThreadFun)func;

 p_info_ = (ThreadInfo*)p_threadInfo;
 int gdwThreadId = 0;
 handle_ = YFCreateThread(NULL, 0, (YFLPTHREAD_START_ROUTINE)ThreadProc, p_info_, 0, (YFLPDWORD)&gdwThreadId);

 return NULL == handle_;
}


} /// namespace YF
#endif /* THREADSPART_H_ */
