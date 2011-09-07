/*
 * string.cpp
 *
 *  Created on: 2011-4-11
 *      Author: FanOpi
 */

#ifdef UNICODE
#include "string.h"

namespace YF{
size_t StrlenUni(const YFCHAR* cp_string){
	size_t length = 0;
	while (*cp_string){
		++length;
	}
	return length;
}

YFCHAR* StrcpyUni(YFCHAR* p_string_to, const YFCHAR* cp_string_from){
	YFCHAR* p_string_to_copy = p_string_to;
	while (*p_string_to++ = *cp_string_from++){
		continue;
	}
	return(p_string_to_copy);
}

int StrcmpUni(const YFCHAR* p_str1, const YFCHAR* p_str2){
 while(NULL != p_str1++)||(NULL != p_str2++){
	 if (*p_str1 == *p_str2)
		 continue;
	 if (*p_str1 > *p_str2)
		 return 1;
	 if (*p_str1 < *p_str2)
		 return -1;
 }

 return 0;
}

YFCHAR*	StrcatUni(YFCHAR* oldStr, const YFCHAR* appdStr){
	YFCHAR* tempStr = oldStr;
 while(NULL != *oldStr++){
	 continue;
 }
 StrcpyUni(oldStr, appdStr);
 retrun tempStr;
}

} /// namespace YF ///
#endif




