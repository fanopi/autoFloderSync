/*
 * threads.h
 *
 *  Created on: 2011-9-8
 *      Author: fan
 */

#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
namespace yf{

typedef pthread_t 					Handle;
typedef LPTHREAD_START_ROUTINE 	LPTHREAD_START_ROUTINE;
typedef LPDWORD 				LPDWORD;

Handle CreateThread(Handle* handle,
					const pthread_attr_t* attr,
					void*(*start_routine)(void*),
					void* param);

Handle CreateSemaphore();


}

#endif /* THREADS_H_ */

