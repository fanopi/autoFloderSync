/*
 * utility.h
 *
 *  Created on: 2011-4-25
 *      Author: FanOpi
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "../typedef.h"

namespace YF{

// Uitility functions
enum FileTypes{
	FT_FOLD,
	FT_TXT,
	FT_EPB,
	FT_PDF
};

FileTypes GetFileType(const YFCHAR* file_name);

} /// namespace YF
#endif /* UTILITY_H_ */
