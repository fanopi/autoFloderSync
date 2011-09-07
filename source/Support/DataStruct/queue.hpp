/*
 * fileNodeQueue.h
 *
 *  Created on: 2011-4-9
 *      Author: Administrator
 */

#ifndef YF_SOURCE_INCLUDE_SUPPORT_OBJECT_QUEUE_H_
#define YF_SOURCE_INCLUDE_SUPPORT_OBJECT_QUEUE_H_

#include "../../os/threads/threads.hpp"

template<typename TENTRY>
class Queue{
 public:
	typedef long ObjectQueueIndex;

	Queue(){
		head_index_ = 0;
		tail_index_ = 0;
		YFInitializeCriticalSection(&lock_);
	}

	~Queue(){
		YFEnterCriticalSection(&lock_);

		YFLeaveCriticalSection(&lock_);
		YFDeleteCriticalSection(&lock_);
	}

	void InQueue(TENTRY file_node) {
		if ( IsQueueFull() )
			return;

		YFEnterCriticalSection(&lock_);
		fileNodeArray_[head_index_] = file_node;
		head_index_ = (head_index_ + 1) % NODE_MAX_NUMBER;
		YFLeaveCriticalSection(&lock_);
	}

	TENTRY OutQueue(){
		TENTRY entry;
		if( IsQueueEmpty())
			return entry; // TODO

		YFEnterCriticalSection(&lock_);
		entry = this->fileNodeArray_[tail_index_];
		tail_index_ = (tail_index_ + 1) % NODE_MAX_NUMBER;
		YFLeaveCriticalSection(&lock_);
		return entry;
	}

	bool IsQueueFull() const {
		return ((tail_index_+1) % NODE_MAX_NUMBER) == NODE_MAX_NUMBER;
	}

	bool IsQueueEmpty()const {
		return tail_index_ == head_index_;
	}

 private:
	const static ObjectQueueIndex NODE_MAX_NUMBER = 40*1024;
	TENTRY fileNodeArray_[NODE_MAX_NUMBER];
	ObjectQueueIndex head_index_;
	ObjectQueueIndex tail_index_;
	mutable YFCRITICAL_SECTION  lock_;
};

#endif /* YF_SOURCE_INCLUDE_SUPPORT_OBJECT_QUEUE_H_ */
