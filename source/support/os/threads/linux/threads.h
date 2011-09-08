/*
 * threads.h
 *
 *  Created on: 2011-9-8
 *      Author: fan
 */

#ifndef THREADS_H_
#define THREADS_H_

namespace yf{
#include <pthread.h>
typedef HANDLE 					Handle;
typedef LPTHREAD_START_ROUTINE 	LPTHREAD_START_ROUTINE;
typedef LPDWORD 				LPDWORD;

// TODO instead using function of macro.
#define CreateThread 				CreateThread
#define InitializeCriticalSection InitializeCriticalSection
#define EnterCriticalSection 		EnterCriticalSection
#define LeaveCriticalSection 		LeaveCriticalSection
#define DeleteCriticalSection 	DeleteCriticalSection
#define CRITICAL_SECTION 			CRITICAL_SECTION
}

#endif /* THREADS_H_ */

