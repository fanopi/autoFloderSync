/*
 * threadsPart.h
 *
 *  Created on: 2011-4-20
 *      Author: FanOpi
 */

#ifndef THREADSPART_H_
#define THREADSPART_H_

#include "../../support/typedef.h"
#ifdef _WIN32 // _LINUX
#include "windows/threads.h"
#else
#include "linux/threads.h"
#endif

namespace yf{

//enclose thread running info
template <typename THREADOBJECT> struct ThreadInfo{
  typedef int (THREADOBJECT::*ThreadFunction)(long);

  THREADOBJECT* 	object;
  ThreadFunction 	function;
  long 				parameter; // parameter of function
};

class NullClass{
  /*nothing to do*/
};
typedef ThreadInfo<NullClass> 					NullThreadInfo;
typedef ThreadInfo<NullClass>::ThreadFunction 	NullThreadFunction;

class YFThread{
 public:
  YFThread(){
    handle_ = (Handle)0;
  };
  ~YFThread(){};
  template<typename THREADOBJECT>
  bool Start(THREADOBJECT* object,NullThreadFunction function,long parameter = 0);

 private:
  static int  ThreadProc(NullThreadInfo* object);

 private:
  Handle 	   handle_;
  ThreadInfo*  threadinfo_;
};

template<typename THREADOBJECT>
bool YFThread::Start(THREADOBJECT* object,NullThreadFunction func, long parameter){
 if(handle_ || NULL == object || NULL == func)
  return false; // thread have running.

 ThreadInfo<THREADOBJECT>* thread_info = new ThreadInfo<THREADOBJECT>;
 thread_info->object = object;
 thread_info->parameter = parameter;
 thread_info->function = (ThreadInfo<THREADOBJECT>::ThreadFunction)func;

 threadinfo_ = (ThreadInfo*)thread_info;
 int gdwThreadId = 0;
 handle_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, threadinfo_, 0, (LPDWORD)&gdwThreadId);

 return NULL == handle_;
}

} /// namespace YF
#endif /* THREADSPART_H_ */
