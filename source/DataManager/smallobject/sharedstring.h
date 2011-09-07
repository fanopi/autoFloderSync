/*
 * fileName.h
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_FILENAME_H_
#define FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_FILENAME_H_

#include "../../include/typedef.h"
#include "../../include/support/classsupport.hpp"
#include "../../include/support/datastruct/map.hpp"
#include "../../include/support/stringhash.h"

namespace YF{

class SharedString{
 public:
	static const long INVALID_STRING_INDEX = -1;
	enum _ERROR_CODE{
		ERROR_INSERT_ENTRY = -1,
		ERROR_ENTRY_NOT_FIND = -2,
		ERROR_FUNCTION_UNKNOWN = -3,
	};
  static SharedString* GetSharedString();
  ~SharedString();
  StringTrait   StoreString(const YFCHAR* P_string);
  long	  GetStringIndex(const StringTrait)const;
	YFCHAR*  		  GetString(long string_index);

 private:
  typedef struct _StringStoreEntry{
  	long  				index;
    StringTrait   trait;
    int           reference_times;
    YFCHAR*       p_string;
  }StringStoreEntry;
  const static int kMaxArraySize = 1000;
  static SharedString* singlenton_;
	SharedString();
	long FindTrait(StringTrait trait);
  int  AddEntryRefenence(long entry_index);
  long SharedString::FindAvailEntry();
  StringStoreEntry* array_[kMaxArraySize];
  DISALLOW_COPY_ADN_ASSIGN(SharedString);
}; /// class StringStore ///

} /// namespace YF
#endif /* FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_FILENAME_H_ */
