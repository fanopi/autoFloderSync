/*
 * threadsPart.h
 *
 *  Created on: 2011-4-20
 *      Author: FanOpi
 */

#ifndef THREADSPART_H_
#define THREADSPART_H_

#include "../../../support/typedef.h"
#ifdef _WIN32 // _LINUX
#include "windows/threads.h"
#else
#include "linux/threads.h"
#endif

namespace yf{

//enclose thread running info
template <typename T>
struct ThreadInfo{
  typedef int (T::*ThreadFunction)(long);

  T* 	            object;
  ThreadFunction 	function;
  long 				    parameter; // parameter of the function
};

/** Noted that thanks of the class is Null,
 * the sizeof(ThreadInfo<NullClass>) == sizeof(struct ThreadInfo).
 * this character is usful for the YFThread.
 */
class NullClass{
  /*nothing to do*/
};
typedef ThreadInfo<NullClass> 					        NullThreadInfo;
typedef ThreadInfo<NullClass>::ThreadFunction 	NullThreadFunction;

class YFThread{
 public:
  YFThread(){
    handle_ = (Handle)0;
  };
  ~YFThread(){};
  template<typename T>
  bool Start(T* object,NullThreadFunction function,long parameter = 0);

 private:
  static int  ThreadProc(NullThreadInfo* object);

 private:
  Handle 	   		  handle_;
  NullThreadInfo* threadinfo_;
};

/** this function using template instance automaticily introcude.
 * 这个函数使用了模板参数自动推导的特性
 */
template<typename T>
bool YFThread::Start(T* object,NullThreadFunction func, long parameter){
  if(handle_ || NULL == object || NULL == func)
    return false; // thread have running.

  ThreadInfo<T>* thread_info = new ThreadInfo<T>;
  thread_info->object = object;
  thread_info->parameter = parameter;
  thread_info->function = (typename ThreadInfo<T>::ThreadFunction)func;

 threadinfo_ = (NullThreadInfo*)thread_info;
#ifdef _WIN32
   int gdwThreadId = 0;
   handle_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, threadinfo_, 0, (LPDWORD)&gdwThreadId);
#else
  int result = pthread_create(&handle_, NULL, (void*(*)(void*))ThreadProc, NULL);
#endif

 return NULL != handle_;
}

} /// namespace YF
#endif /* THREADSPART_H_ */
