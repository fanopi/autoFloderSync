/*
 * stdio.h
 *
 *  Created on: 2011-4-27
 *      Author: FanOpi
 */
#ifndef YF_SOURCE_INCLUDE_LIBRATY_STDIO_H_
#define YF_SOURCE_INCLUDE_LIBRATY_STDIO_H_

#ifdef UNICODE
#include "../typedef.h"
namespace YF{
FILE* 	FopenUni(const YFCHAR* cp_filename, const YFCHAR* mode);
int    	FcloseUni(const YFCHAR* cp_filename);
int			FputsUni(const YFCHAR* string, FILE* stream);
} /// namespace YF ///
#else
#include <stdio.h>
#define FopenUni  ::fopen
#define FcloseUni ::fclose
#define FputsUni  ::fputs
#endif /// UNICODE ///


#endif /* YF_SOURCE_INCLUDE_LIBRATY_STDIO_H_ */
