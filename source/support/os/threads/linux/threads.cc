/*
 * threads.cc
 *
 *  Created on: 2011-9-9
 *      Author: fan
 */
#include "threads.h"
namespace yf{

typedef pthread_t 					Handle;
typedef LPTHREAD_START_ROUTINE 	LPTHREAD_START_ROUTINE;
typedef LPDWORD 				LPDWORD;

Handle CreateThread(Handle* handle,
					const pthread_attr_t* attr,
					void*(*start_routine)(void*),
					void* param){
	return (Handle)::pthread_creat(handle, attr, start_routine, param);
}

}



