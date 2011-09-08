/*
 * stdlib.h
 * 增强的内存分配器
 * 1）当开启__MEMORY_DEBUG__时，可以自动记录内存分配的对象和文件，判断其是否成功的释放了所分配的内存。
 * 2）采用类似于SGI STL 的内存分配方式来对内存进行管理，当分配的内存较小时，可以有效的避免内存碎片。
 *  Created on: 2011-4-11
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_LIBRATY_STDLIB_H_
#define FPJ_SOURCE_INCLUDE_LIBRATY_STDLIB_H_

#include <stdlib.h>
#include "../typedef.h"

namespace YF{
#if 0
void* malloc(size_t size, const YFCHAR* file = __FILE__, int line = __LINE__);
void 	free(void* memory);
void* operator new(size_t size, const YFCHAR* file = __FILE__, int line = __LINE__);
void* operator new[](size_t size, const YFCHAR* file = __FILE__, int line = __LINE__);
void 	operator delete(void *block);
void 	operator delete[](void *block);

////////////////////////////////////////////////////////////////////////////////
class ChunkAlloc{
 public:
	static void* Allocate(size_t n){
		void* result = ::malloc(n);
		if (NULL == result)
			result = ChunkAlloc::OomAlloc(n);
		return result;
	}

	static void Deallocate(void* p, size_t n){
			::free(p);
	}

	static void* Reallocate(void* p, size_t /* old size*/, size_t newSize){
		void* result = ::realloc(p, newSize);
		if (NULL == result)
			result = ChunkAlloc::OomRealloc(p, newSize);
		return result;
	}

	// the return value is function pointer
	static void*(* SetOomHandle(void* oomHandle()))(){
		void*(*old)() = ChunkAllocOomHandle_;
		ChunkAllocOomHandle_ = oomHandle;
		return old;
	}

 private:
	static void* (*ChunkAllocOomHandle_)();
	static void* OomAlloc(size_t n);
	static void* OomRealloc(void*, size_t);
}; ///~ class ChunkMalloc ~///

class DefaultAlloc{
	public:
		enum {
			_ALIGN 			= 8,
			_MAX_BYTES 	= 128,
			_NUMBER_FREELISTS = _MAX_BYTES / _ALIGN
		};
		static void* Allocate(size_t alloc_size);
		static void  Deallocate(void* p, size_t alloc_size);
		static void* Reallocate(void* p, size_t /* old size*/, size_t new_size);

	private:
		union FreeListEntry{
			union FreeListEntry* p_next_entry;
			char 	client_data[1];
		};

		static size_t FREELIST_INDEX(size_t bytes){
			return ( ((bytes) + _ALIGN - 1) / _ALIGN - 1);
		}

		static size_t ROUND_UP(size_t n){
			return (n + _ALIGN - 1) & ~(_ALIGN - 1);
		}

		static void* Refill(size_t refill_list_entry_size);
		static char* ChunkAlloc(size_t size, int& r_nobjs);
		static FreeListEntry* free_list_[_NUMBER_FREELISTS];
		static char* s_start_free_;
		static char* s_end_free_;
		static size_t s_heap_size_;
};
//~ class MemoryPoolMalloc ~//
#endif
} /// namespace YF ///
#endif /* FPJ_SOURCE_INCLUDE_LIBRATY_STDLIB_H_ */
