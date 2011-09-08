/*
 * string.h
 *
 *  Created on: 2011-5-4
 *      Author: 代码基于 TinyXML项目 重构 原作者Lee Thomason
 */

#ifndef TIXML_STRING_INCLUDED
#define TIXML_STRING_INCLUDED

#include <assert.h>
#include "../../library/string.h"

namespace YF{

#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
#define EXPLICIT explicit
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
#define EXPLICIT explicit
#else
#define EXPLICIT
#endif

/* String is an emulation of a subset of the std::string template.
 * The buffer allocation is made by a simplistic power of 2 like mechanism :
 * if we increase a string and there's no more room, we allocate a buffer twice as big as we need.*/
class String{
	public:
		typedef size_t SIZE;
		static const SIZE npos; // Error value for find primitive
		String() : rep_( &nullrep_) { }
		String(const String & copy) : rep_(0){
			init(copy.length( ));
			YFmemcpy(start( ), copy.data( ), length( ));
		}

		EXPLICIT String(const YFCHAR * copy) : rep_(0){
			init(static_cast < SIZE > (StrlenUni(copy)));
			YFmemcpy(start( ), copy, length( ));
		}

		// String constructor, based on a string
		EXPLICIT String(const YFCHAR * str, SIZE len) :	rep_(0){
			init(len);
			YFmemcpy(start( ), str, len);
		}

		~String(){
			quit( );
		}

		String& operator =(const YFCHAR * copy){
			return assign(copy, (SIZE) StrlenUni(copy));
		}

		String& operator =(const String & copy){
			return assign(copy.start( ), copy.length( ));
		}

		String& operator +=(const YFCHAR * suffix){
			return append(suffix, static_cast < SIZE > (StrlenUni(suffix)));
		}

		String& operator +=(YFCHAR single){
			return append( &single, 1);
		}

		String& operator +=(const String & suffix){
			return append(suffix.data( ), suffix.length( ));
		}

		const YFCHAR * c_str() const {
			return rep_->str;
		}

		const YFCHAR * data() const {
			return rep_->str;
		}

		SIZE length() const {
			return rep_->size;
		}

		SIZE size() const {
			return rep_->size;
		}

		bool empty() const {
			return rep_->size == 0;
		}

		SIZE capacity() const {
			return rep_->capacity;
		}

		const YFCHAR& at(SIZE index) const{
			assert(index < length( ));
			return rep_->str[index];
		}

		YFCHAR& operator [](SIZE index) const{
			assert(index < length( ));
			return rep_->str[index];
		}

		SIZE find(YFCHAR lookup) const{
			return find(lookup, 0);
		}

		SIZE find(YFCHAR tofind, SIZE offset) const{
			if (offset >= length( )) return npos;

			for (const YFCHAR* p = c_str( ) + offset; *p != '\0'; ++p){
				if ( *p == tofind)
					return static_cast < SIZE > (p - c_str( ));
			}
			return npos;
		}

		void clear(){
			quit( );
			init(0, 0);
		}


		/*	Function to reserve a big amount of data when we know we'll need it.
		 *  Be aware that this function DOES NOT clear the content of the String if any exists.	*/
		void reserve(SIZE cap){
			if (cap > capacity( )){
				String tmp;
				tmp.init(length( ), cap);
				YFmemcpy(tmp.start( ), data( ), length( ));
				swap(tmp);
			}
		}

		String& assign(const YFCHAR* str, SIZE len){
			SIZE cap = capacity( );
			if (len > cap || cap > 3 * (len + 8)){
				String tmp;
				tmp.init(len);
				YFmemcpy(tmp.start( ), str, len);
				swap(tmp);
			}else{
				YFmemmove(start( ), str, len);
				set_size(len);
			}
			return *this;
		}

		String& append(const YFCHAR* str, SIZE len){
			SIZE newsize = length( ) + len;
			if (newsize > capacity( )){
				reserve(newsize + capacity( ));
			}
			YFmemmove(finish( ), str, len);
			set_size(newsize);
			return *this;
		}
		
		void swap(String& other){
			Rep* r = rep_;
			rep_ = other.rep_;
			other.rep_ = r;
		}

	private:
		void init(SIZE sz){
			init(sz, sz);
		}

		void set_size(SIZE sz){
			rep_->str[rep_->size = sz] = '\0';
		}

		YFCHAR* start() const {
			return rep_->str;
		}

		YFCHAR* finish() const {
			return rep_->str + rep_->size;
		}

		struct Rep {
				SIZE size;
				SIZE capacity;
				YFCHAR str[1];
		};

		void init(SIZE sz, SIZE cap){
			if (cap){
				const SIZE bytesNeeded = sizeof(Rep) + cap;
				const SIZE intsNeeded = (bytesNeeded + sizeof(int) - 1) / sizeof(int);
				rep_ = reinterpret_cast < Rep* > (new int[intsNeeded]);

				rep_->str[rep_->size = sz] = '\0';
				rep_->capacity = cap;
			}else{
				rep_ = &nullrep_;
			}
		}

		void quit(){
			if (rep_ != &nullrep_){
				delete[] (reinterpret_cast < int* > (rep_));
			}
		}

		Rep * rep_;
		static Rep nullrep_;
};

inline bool operator ==(const String & a, const String & b){
	return (a.length( ) == b.length( )) // optimization on some platforms
	    && (StrcmpUni(a.c_str( ), b.c_str( )) == 0); // actual compare
}

inline bool operator <(const String & a, const String & b) { return StrcmpUni(a.c_str( ), b.c_str( )) < 0;}

inline bool operator !=(const String & a, const String & b) { return !(a == b);}

inline bool operator >(const String & a, const String & b) { return b < a;}

inline bool operator <=(const String & a, const String & b) { return !(b < a);}

inline bool operator >=(const String & a, const String & b) { return !(a < b);}

inline bool operator ==(const String & a, const YFCHAR* b) {	return StrcmpUni(a.c_str( ), b) == 0;}

inline bool operator ==(const YFCHAR* a, const String & b) {	return b == a;}

inline bool operator !=(const String & a, const YFCHAR* b) {	return !(a == b);}

inline bool operator !=(const YFCHAR* a, const String & b) {	return !(b == a);}

String operator +(const String & a, const String & b);

String operator +(const String & a, const YFCHAR* b);

String operator +(const YFCHAR* a, const String & b);

} /// namespace YF

#endif	// TIXML_STRING_INCLUDED
