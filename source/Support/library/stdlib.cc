/*
 * stdlib.cc
 * 增强的内存分配器
 * 1）当开启__MEMORY_DEBUG__时，可以自动记录内存分配的对象和文件，判断其是否成功的释放了所分配的内存。
 * 2）采用类似于SGI STL 的内存分配方式来对内存进行管理，当分配的内存较小时，可以有效的避免内存碎片。
 *  Created on: 2011-4-11
 *      Author: FanOpi
 */

#include <stdlib.h>
#include "stdlib.h"
#include <assert.h>

#define __MEMORY_DEBUG__

#ifdef __MEMORY_DEBUG__
#include "../support/memoryleak/memoryleaktrace.h"
#endif

namespace YF{
#if 0
#ifndef THROW_BAD_ALLOC
#define THROW_BAD_ALLOC
#endif

void* malloc(size_t size, const YFCHAR* file, int line){
#ifdef __MEMORY_DEBUG__
	void* p_address = DefaultAlloc::Allocate(size);
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	p_memoryLeakTrace->Trace(MEMORY_MALLOC, p_address, size, file, line);
	return p_address;
#else
	return ::malloc(size);
#endif
}

void free(void* memory){
#ifdef __MEMORY_DEBUG__
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	size_t size =	p_memoryLeakTrace->Detrace(MEMORY_FREE, memory);
	DefaultAlloc::Deallocate(memory, size);
#else
	::free(memory);
#endif
}

void* operator new(size_t size, const YFCHAR* file, int line){
#ifdef __MEMORY_DEBUG__
	void* p_address = DefaultAlloc::Allocate(size);
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	p_memoryLeakTrace->Trace(MEMORY_NEW, p_address, size, file, line);
	return p_address;
#else

#endif
}

void* operator new[](size_t size, const YFCHAR* file, int line){
#ifdef __MEMORY_DEBUG__
	void* p_address = DefaultAlloc::Allocate(size);
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	p_memoryLeakTrace->Trace(MEMORY_NEW, p_address, size, file, line);
	return p_address;
#else

#endif

}

void operator delete(void *memory){
#ifdef __MEMORY_DEBUG__
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	size_t size =	p_memoryLeakTrace->Detrace(MEMORY_DELETE, memory);
	DefaultAlloc::Deallocate(memory, size);
#else

#endif
}

void operator delete[](void *memory){
#ifdef __MEMORY_DEBUG__
	MemoryLeakTrace* p_memoryLeakTrace = MemoryLeakTrace::GetMemoryLeakTrace();
	size_t size =	p_memoryLeakTrace->Detrace(MEMORY_DELETE, memory);
	DefaultAlloc::Deallocate(memory, size);
#else

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// class ChunkMalloc ///
void* (*ChunkAlloc::ChunkAllocOomHandle_)();
void* ChunkAlloc::OomAlloc(size_t n){
	void* (*myMallocHandle)();
	void* result;
	for( ; ; ){
		myMallocHandle = ChunkAllocOomHandle_;
		if (static_cast<void* (*)()>(NULL) == myMallocHandle){
			THROW_BAD_ALLOC;
		}
		(* myMallocHandle)();
		result = ::malloc(n);
		if(NULL != result)
			return result;
	}
}

void* ChunkAlloc::OomRealloc(void* p, size_t newSize){
	void* (*myMallocHandle)();
	void* result;
	for( ; ; ){
		myMallocHandle = ChunkAllocOomHandle_;
		if (static_cast<void* (*)()>(NULL) == myMallocHandle){
			THROW_BAD_ALLOC; // TODO(fpj):
		}
		(* myMallocHandle)();
		result = ::realloc(p, newSize);
		if(NULL != result)
			return result;
	}
}/// class ChunkMalloc ///

/// class MemoryPoolMalloc ///
char* 	DefaultAlloc::s_start_free_ 	= NULL;
char* 	DefaultAlloc::s_end_free_ 		= NULL;
size_t 	DefaultAlloc::s_heap_size_ 		= NULL;
DefaultAlloc::FreeListEntry* DefaultAlloc::free_list_[] = {
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

void* DefaultAlloc::Allocate(size_t alloc_size){
	FreeListEntry** pp_my_free_list;
	FreeListEntry* 	p_result;

	if (alloc_size > (size_t)_MAX_BYTES)
		return (ChunkAlloc::Allocate(alloc_size));
	pp_my_free_list = free_list_ + FREELIST_INDEX(alloc_size);
	p_result = *pp_my_free_list;
	if (NULL == p_result){
		void *p_rtn = DefaultAlloc::Refill(ROUND_UP(alloc_size));
		return p_rtn;
	}else{
		*pp_my_free_list = p_result->p_next_entry;
		return static_cast<void*>(p_result);
	}
}

void DefaultAlloc::Deallocate(void* p_dealloc, size_t dealloc_size){
	FreeListEntry** pp_my_free_list;
	FreeListEntry* p_deallocate = static_cast<FreeListEntry*>(p_dealloc);

	if (dealloc_size > (size_t)_MAX_BYTES)
		return ChunkAlloc::Deallocate(p_dealloc, dealloc_size);
	pp_my_free_list = free_list_ + FREELIST_INDEX(dealloc_size);
	p_deallocate->p_next_entry = *pp_my_free_list;
	(*pp_my_free_list)->p_next_entry = p_deallocate;
}

void* DefaultAlloc::Reallocate(void* p, size_t /*old size*/, size_t newSize){
	// TODO(fpj):
	return NULL;
}

void* DefaultAlloc::Refill(size_t refill_list_entry_size){
	int 	r_list_entry_number = 20;
	char* p_chunk = ChunkAlloc(refill_list_entry_size, r_list_entry_number);
	if (0 == r_list_entry_number)
		return NULL;
	if (1 == r_list_entry_number)
		return p_chunk;

	FreeListEntry* p_current_entry;
	FreeListEntry* p_next_entry;
	FreeListEntry* volatile* pp_my_free_list = free_list_ + FREELIST_INDEX(refill_list_entry_size);
	*pp_my_free_list = p_next_entry = (FreeListEntry*)(p_chunk + refill_list_entry_size);
	for(int entryIndex = 1; 	; entryIndex++){
		p_current_entry = p_next_entry;
		p_next_entry 		= (FreeListEntry*)((char*)(p_next_entry) + refill_list_entry_size);
		if (r_list_entry_number - 1 == entryIndex){
			p_current_entry->p_next_entry = NULL;
			break;
		}else{
			p_current_entry->p_next_entry = p_next_entry;
		}//~ if (r_list_....
	}//~ for(int entryIndex....

	return (FreeListEntry*)(p_chunk);
}

char* DefaultAlloc::ChunkAlloc(size_t refill_list_entry_size, int& r_list_entry_number){
	char* p_result;
	size_t total_bytes = refill_list_entry_size * r_list_entry_number;
	size_t left_bytes = s_end_free_ - s_start_free_;

	if (left_bytes > total_bytes){
		p_result = s_start_free_;
		s_start_free_ += total_bytes;
		return p_result;
	}else if (left_bytes >= refill_list_entry_size){
		r_list_entry_number = left_bytes / refill_list_entry_size;
		p_result = s_start_free_;
		s_start_free_ += total_bytes;
		return p_result;
	}else{
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(s_heap_size_>>4);
		if (left_bytes > 0){
			FreeListEntry** pp_my_free_list = free_list_ + FREELIST_INDEX(left_bytes);
			((FreeListEntry*)s_start_free_)->p_next_entry = *pp_my_free_list;
			*pp_my_free_list = (FreeListEntry*)s_start_free_;
		}
		s_start_free_ = (char*)malloc(bytes_to_get);
		if (NULL == s_start_free_){
			FreeListEntry** pp_m_free_list;
			FreeListEntry* p_list_entry;
			for(int list_entry_size = refill_list_entry_size; list_entry_size <= _MAX_BYTES; list_entry_size+=_ALIGN){
				pp_m_free_list = free_list_ + FREELIST_INDEX(list_entry_size);
				p_list_entry = *pp_m_free_list;
				if (NULL != p_list_entry){
					*pp_m_free_list = p_list_entry->p_next_entry;
					s_start_free_ 	= (char*)p_list_entry;
					s_end_free_ 		= s_start_free_ + list_entry_size;
					return ChunkAlloc(refill_list_entry_size, r_list_entry_number);
				}
			}//~ for(i = size; i....
		s_end_free_ = 0;
		s_start_free_ = (char*)ChunkAlloc::Allocate(bytes_to_get);
		}//~ if (NULL == st....

		s_heap_size_ += bytes_to_get;
		s_end_free_ = s_start_free_ + bytes_to_get;
		return ChunkAlloc(refill_list_entry_size, r_list_entry_number);
	}//~else
}/// class MemoryPoolMalloc ///
#endif
} /// namespace YF
