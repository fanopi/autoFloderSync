/*
 * autoPtr.h
 *
 *  Created on: 2011-4-18
 *      Author: Administrator
 */

#ifndef FPJ_SOURCE_INCLUDE_LIBRATY_AUTOPTR_H_
#define FPJ_SOURCE_INCLUDE_LIBRATY_AUTOPTR_H_

#include "../typedef.h"

template<typename ObjType>
class ScopedPtr{
 public:
	ScopedPtr(ObjType* p_obj): p_obj_(p_obj){

	}

	~ScopedPtr(){
		if (NULL != p_obj_){
			delete p_obj_;
		}
	}

	ObjType* operator ->(){
		return p_obj_;
	}

	ObjType operator *(){
		return *p_obj_;
	}

	ObjType* Release() const{
		ObjType* p_temp = p_obj_;
		(const_cast<ScopedPtr<ObjType>*>(this))->p_obj_ = NULL;
		return p_temp;
	}

private:
	ScopedPtr();
	// ok, yes, it's meaningless..
	ScopedPtr<ObjType>& operator = (const ScopedPtr<ObjType>& r_scoped_obj){
		if (this != &r_scoped_obj){
			delete this->p_obj_;
			p_obj_ = r_scoped_obj.Release();
		}
		return *this;
	}

	ObjType* p_obj_;
};

#endif /* FPJ_SOURCE_INCLUDE_LIBRATY_AUTOPTR_H_ */
