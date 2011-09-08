/*
 * fileName.cpp
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#include "sharedstring.h"
#include "../../include/library/stdlib.h"
#include "../../include/library/string.h"

namespace YF{

SharedString* SharedString::singlenton_ = NULL;
SharedString* SharedString::GetSharedString(){
	if (NULL == SharedString::singlenton_)
		SharedString::singlenton_ = new SharedString;

	return SharedString::singlenton_;
}

SharedString::SharedString(){
  for(long index = 0; index < kMaxArraySize; index++){
	  array_[index] = NULL;
  }
}

SharedString::~SharedString(){
  StringStoreEntry* p_string_store_entry = NULL;
  for(long index=0; index < kMaxArraySize; index++){
  	p_string_store_entry = array_[index];
  	if (NULL == p_string_store_entry){
  		if(NULL != p_string_store_entry->p_string)
  			free(p_string_store_entry->p_string);

  		free(p_string_store_entry);
  	}
  }/// for
}

StringTrait SharedString::StoreString(const YFCHAR* P_string){
	StringTrait string_trait;
	GetStringTrait(P_string, &string_trait);
	long index = this->FindTrait(string_trait);
  if (ERROR_ENTRY_NOT_FIND != index){
    return string_trait;
  }else{
  	index = this->FindAvailEntry();
  	if (ERROR_ENTRY_NOT_FIND == index)  goto ERROR_EXIT;
  	StringStoreEntry* p_string_store_entry = (StringStoreEntry*)malloc(sizeof(StringStoreEntry));
  	this->array_[index] = p_string_store_entry;
  	p_string_store_entry->index = index;
  	p_string_store_entry->trait = string_trait;
  	p_string_store_entry->reference_times = 1;
  	p_string_store_entry->p_string = (YFCHAR*)malloc(sizeof(YFCHAR)*string_trait.length);
  	if (NULL == p_string_store_entry->p_string)  goto ERROR_EXIT;
  	StrcpyUni(p_string_store_entry->p_string, P_string);
  	return string_trait;
  }
ERROR_EXIT:
  	return string_trait;
}

long SharedString::GetStringIndex(const StringTrait trait) const {
  StringStoreEntry* p_string_store_entry = NULL;
  for(long index=0; index < kMaxArraySize; index++){
  	p_string_store_entry = array_[index];
	  if ( NULL !=	p_string_store_entry && p_string_store_entry->trait == trait)
		return index;
  }
  return ERROR_ENTRY_NOT_FIND;
}

YFCHAR* SharedString::GetString(long index){
  StringStoreEntry* p_string_store_entry = NULL;
  for(long i=0; i < kMaxArraySize; i++){
  	p_string_store_entry = array_[i];
	  if ( NULL !=	p_string_store_entry && index == p_string_store_entry->index)
		return p_string_store_entry->p_string;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
long SharedString::FindTrait(StringTrait trait){
  StringStoreEntry* p_string_store_entry;
  for(long index=0; index < kMaxArraySize; index++){
  	p_string_store_entry = array_[index];
	  if(   NULL !=	p_string_store_entry
	  	&& (trait == p_string_store_entry->trait))
		return index;
  }
  return ERROR_ENTRY_NOT_FIND;
}

long SharedString::FindAvailEntry(){
	long array_first_avail_index = -1;
  StringStoreEntry* p_string_store_entry = NULL;
  for(long index=0; index < kMaxArraySize; index++){
  	p_string_store_entry = array_[index];
  	if ( NULL == p_string_store_entry)
  		return index;
	  if (NULL != p_string_store_entry && 0 == p_string_store_entry->reference_times)
		  return index;
  }
  return ERROR_ENTRY_NOT_FIND;
}

int SharedString::AddEntryRefenence(long entry_index)
{
	StringStoreEntry* p_string_store_entry = this->array_[entry_index];
	return ++p_string_store_entry->reference_times;
}

} /// namespace YF
