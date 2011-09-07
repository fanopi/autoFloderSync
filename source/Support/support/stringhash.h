/*
 * hash.h
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_OBJECT_HASH_H_
#define FPJ_SOURCE_INCLUDE_OBJECT_HASH_H_

#include "../typedef.h"
namespace YF{

typedef struct StringTrait{
 public:
	int length; //has added 1 for the eof '\0'
	int hash1;
	int hash2;
	int hash3;

 public:
	bool operator ==(const StringTrait& trait){
  	return 	 (trait.hash1 == this->hash1)
  				&& (trait.hash2 == this->hash2)
  				&& (trait.hash3 == this->hash3)
  				&& (trait.length == this->length);
	}
	bool operator ==(const StringTrait* trait){
  	return 	 (trait->hash1 == this->hash1)
  				&& (trait->hash2 == this->hash2)
  				&& (trait->hash3 == this->hash3)
  				&& (trait->length == this->length);
	}
	StringTrait operator = (const StringTrait& trait){
		this->length = trait.length;
		this->hash1 = trait.hash1;
		this->hash2 = trait.hash2;
		this->hash3 = trait.hash3;
		return  *this;
	}
}StringTrait;

void GetStringTrait(const YFCHAR* p_string, StringTrait* p_string_trait);
} /// namespace YF
#endif /* FPJ_SOURCE_INCLUDE_OBJECT_HASH_H_ */
