/*
 * hash.h
 *
 *  Created on: 2011-4-11
 *      Author: FanOpi
 */

#ifndef __YF_INCLUDE_SUPPORT_HASHTABLE_H_
#define __YF_INCLUDE_SUPPORT_HASHTABLE_H_

template<typename TENTRY>
class HashTable{
 public:
	typedef long HashTableIndex;
	typedef enum _ErrorCode{
		ERROR_SUCCESSFUL = 0,
		ERROR_NOT_EXIST = -1,
		ERROR_INDEX_OVERFOLE = -2,
		ERROR_NOT_FIND = -3,
	}ErrorCode;

	HashTable(){
	}

	void SetHashMap(HashTableIndex index, TENTRY node){
		if ((index > HashTable::NODE_MAX_NUMBER) && (index < 0))
			lastError_ = ERROR_INDEX_OVERFOLE;
		else
			this->hashMap_[index] = node;
	}

	// TODO(fpj): call node.getHashTrait()
	HashTableIndex SearchIndex(TENTRY node){
		for(HashTableIndex i = 0; i < HashTable::NODE_MAX_NUMBER; i++){
			if (this->hashMap_[i] == node)
				return i;
		}
		lastError_ = ERROR_NOT_FIND;
		return knode_not_find; // not exsit.
	}

	ErrorCode GetLashError(){
		ErrorCode return_lastError_ = lastError_;
		lastError_ = ERROR_SUCCESSFUL;
		return return_lastError_;
	}

 private:
	const static HashTableIndex knode_not_find = -1;
	const static HashTableIndex NODE_MAX_NUMBER = 40*1024;
	TENTRY hashMap_[NODE_MAX_NUMBER];
	ErrorCode lastError_;
};
#endif /* __YF_INCLUDE_SUPPORT_HASHTABLE_H_ */
