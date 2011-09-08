/*
 * typedef.h
 * $$ define global types(such as char and function return).
 *  Created on: 2011-4-9
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_TYPEDEF_H_
#define FPJ_SOURCE_INCLUDE_TYPEDEF_H_
namespace yf{

#ifdef UNICODE
typedef unsigned short 	YFCHAR;
#else
typedef char	YFCHAR;
#endif

typedef enum _Status
{
	OK,
	FAIL,
	UNKNOWN
}Status;

typedef unsigned long YFDWORD;
typedef unsigned int 	YFWORD;

typedef struct _TFILETIME {
	YFDWORD dwLowDateTime;
	YFDWORD dwHighDateTime;
}FILETIME;

typedef struct _TSYSTEMTIME {
	YFWORD wYear;
	YFWORD wMonth;
	YFWORD wDayOfWeek;
	YFWORD wDay;
	YFWORD wHour;
	YFWORD wMinute;
	YFWORD wSecond;
	YFWORD wMilliseconds;
}YFSYSTEMTIME;


#define NULL  (void*)0


typedef unsigned int size_t;
}
#endif /* FPJ_SOURCE_INCLUDE_TYPEDEF_H_ */
