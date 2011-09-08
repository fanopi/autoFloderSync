/*
 * threads.h
 *
 *  Created on: 2011-4-23
 *      Author: FanOpi
 */

#ifndef THREADS_H_
#define THREADS_H_

#ifdef _WIN32

#include <windows.h>

typedef HANDLE 					YFHANDLE;
typedef LPTHREAD_START_ROUTINE 	YFLPTHREAD_START_ROUTINE;
typedef LPDWORD 				YFLPDWORD;

// TODO instead using function of macro.
#define YFCreateThread 				CreateThread
#define YFInitializeCriticalSection InitializeCriticalSection
#define YFEnterCriticalSection 		EnterCriticalSection
#define YFLeaveCriticalSection 		LeaveCriticalSection
#define YFDeleteCriticalSection 	DeleteCriticalSection
#define YFCRITICAL_SECTION 			CRITICAL_SECTION

#endif /* THREADS_H_ */

#endif /// _WIN32
