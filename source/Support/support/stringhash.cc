/*
 * hash.cpp
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#include "stringhash.h"
#include "../library/string.h"
namespace YF{

static int Hash(int hash_seed, const YFCHAR* str){
	int seed = hash_seed; //31 131 1313 13131 131313 etc..
	int hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return hash;
}

void GetStringTrait(const YFCHAR* p_string, StringTrait* p_string_trait){
	if ((NULL == p_string) || (NULL == p_string_trait) )
		goto EXIT;
	p_string_trait->length = StrlenUni(p_string) + 1; // add 1 for store EOF'\0'
	p_string_trait->hash1  = Hash(131, p_string);
	p_string_trait->hash2  = Hash(1313, p_string);
	p_string_trait->hash3  = Hash(13131, p_string);
EXIT:
	return;
}

} /// namespace YF
