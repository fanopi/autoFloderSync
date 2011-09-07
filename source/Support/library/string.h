/*
 * string.h
 *
 *  Created on: 2011-4-11
 *      Author: FanOpi
 */

#ifndef YF_SOURCE_INCLUDE_LIBRATY_STRING_H_
#define YF_SOURCE_INCLUDE_LIBRATY_STRING_H_

#include <string.h>

#ifdef UNICODE
#include "../typedef.h"
namespace YF{
size_t 	StrlenUni(const YFCHAR* cp_string);
YFCHAR* StrcpyUni(YFCHAR* p_string_to, const YFCHAR* cp_string_from);
int 		StrcmpUni(const YFCHAR* str1, const YFCHAR* str2);
YFCHAR*	StrcatUni(const YFCHAR* str1, const YFCHAR* str2);
} /// namespace YF ///
#else
#define StrlenUni ::strlen
#define StrcpyUni ::strcpy
#define StrcmpUni ::strcmp
#define StrcatUni ::strcat
#endif /// UNICODE ///

#define YFmemcpy ::memcpy
#define YFmemmove ::memmove

#endif /* YF_SOURCE_INCLUDE_LIBRATY_STRING_H_ */
