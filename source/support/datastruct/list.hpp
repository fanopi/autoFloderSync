/*
 * list.hpp
 *
 *  Created on: 2011-4-23
 *      Author: FanOpi
 */

#ifndef __YF_INCLUDE_SUPPORT_LIST_HPP__
#define __YF_INCLUDE_SUPPORT_LIST_HPP__

namespace YF{

template<typename T>
class List{
 public:
	typedef T 			VALUE;
	typedef T* 			POINT;
	typedef T* 			ITERA;
	typedef T& 			REFER;
	typedef size_t 	SIZE;
	List();
	~List();
	POINT Begin();
	POINT End();
	SIZE Size();
	SIZE Capacity();
	bool Empty();
	VALUE operator[](int n);
	ITERA Erase(ITERA n);
	void Clear();
	void Resize(Size new_size);
	void Pushback(const REFER);
	REFER Popback() const;

 private:
	Iterator start_;
	Iterator finish_;
};

} /// namespace YF
#endif /* __YF_INCLUDE_SUPPORT_LIST_HPP__ */
