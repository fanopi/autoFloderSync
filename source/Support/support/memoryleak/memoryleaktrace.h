/*
 * memoryLeak.h
 *
 *  Created on: 2011-4-21
 *      Author: FanOpi
 */

#ifndef MEMORYLEAK_H_
#define MEMORYLEAK_H_

#include "../../typedef.h"

namespace YF{

typedef struct _MemTraceEntry{
  long		serialnumber;
  size_t	size;
  void*		address;
	YFCHAR* file;
	int			line;
}MemTraceEntry;

typedef enum _TraceType{
	MEMORY_MALLOC,
	MEMORY_FREE,
	MEMORY_NEW,
	MEMORY_DELETE,
}TraceType;

class MemoryLeakTrace{ // singleton object
 public:
	~MemoryLeakTrace();
	static MemoryLeakTrace* GetMemoryLeakTrace();
	void Trace(TraceType type, void* p_address, size_t size, const YFCHAR* p_file = '\0', int line = 0);
	size_t Detrace(TraceType type, void* p_address);
	void Report(const YFCHAR* output_file_name = "c:\\memoryleakTrace.txt");

 private:
	static const int RECORD_SIZE = 60000;
	static MemoryLeakTrace* s_singleton_;
	long serial_;
	MemTraceEntry* mallocRecord_[RECORD_SIZE];
	MemTraceEntry* newRecord_[RECORD_SIZE];
	MemoryLeakTrace();
	void PutInRecord(MemTraceEntry* p_trace_record[], MemTraceEntry* p_ernty);
	MemTraceEntry* MemoryLeakTrace::GetRecord(MemTraceEntry* trace_record[], void* p_address);
	void CancelRecord(MemTraceEntry* trace_record[], MemTraceEntry* p_ernty);
};

} /// namespace YF
#endif MEMORYLEAK_H_
