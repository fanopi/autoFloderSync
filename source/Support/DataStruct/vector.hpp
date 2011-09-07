/*
 * vector.hpp
 *
 *  Created on: 2011-4-23
 *      Author: FanOpi
 */

#ifndef __YF_INCLUDE_SUPPORT_VECTOR_HPP_
#define __YF_INCLUDE_SUPPORT_VECTOR_HPP_

#include "../../library/stdlib.h"
#include "stlsupport.hpp"

namespace YF{

template<typename T>
class Vector{
 public:
	typedef T 			VALUE;
	typedef T* 			POINT;
	typedef T* 			ITERA;
	typedef T& 			REFER;
	typedef size_t 	SIZE;
	const static int INIT_VECTOR_SIZE = 1000;
	const static int INIT_RESERVE_SIZE = 0.25 * INIT_VECTOR_SIZE;
	typedef enum _ErrorCode{
		ERROR_SUCCESSFUL = 0,
		ERROR_NOT_EXIST = -1,
		ERROR_INDEX_OVERFOLE = -2,
		ERROR_NOT_FIND = -3,
		ERROR_INSERT_FULL_QUEUE = -4,
		ERROR_GET_EMPTY_QUEUE = -5,
		ERROR_MALLOC_FAIL = -6,
	}ErrorCode;

	Vector(){
		start_ = (POINT) malloc(sizeof(VALUE) * (INIT_VECTOR_SIZE + INIT_RESERVE_SIZE));
		if (NULL == start_)
			last_error_ = ERROR_MALLOC_FAIL;
		finish_ = start_;
		storage_end_ = start_ + INIT_VECTOR_SIZE;
		curr_capacity_ = INIT_VECTOR_SIZE;
	}

	~Vector(){
		if (NULL == start_)
			 free(start_);
	}

	ITERA Begin(){
		return start_;
	}

	ITERA End(){
		return finish_;
	}

	SIZE Size(){
		return finish_- start_;
	}

	SIZE Capacity(){
		return curr_capacity_;
	}

	bool Empty(){
		return start_ == finish_;
	}

	VALUE operator[](int n){
		return *(start_+ n);
	}

	ITERA Erase(ITERA n){
		int diff = End() - n;
		while (0 != diff){
			*(start_+diff) = *(start_+diff+1);
			diff--;
		}
		Destroy(finish_);
		finish_--;
	}

	void Clear(){
		for(int diff = Size(); diff!=0; diff--){
			// destroy(start_ + diff);
		}
		finish_ = start_;
	}

	void Resize(SIZE new_size){
		// TODO
	}

	void Pushback(const REFER node){
		Construct(finish_ ,node);
		finish_++;
	}

	REFER Popback() const{
		return finish_;
		finish_--;
	}

	ErrorCode GetLashError(){
		ErrorCode return_last_error_ = last_error_;
		last_error_ = ERROR_SUCCESSFUL;
		return return_last_error_;
	}

 private:
	ITERA start_;
	ITERA finish_;
	ITERA storage_end_;
	SIZE curr_capacity_;
	ErrorCode last_error_;
};

} /// namespace YF
#endif /* __YF_INCLUDE_SUPPORT_VECTOR_HPP_ */
