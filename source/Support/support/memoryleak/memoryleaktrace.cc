/*
 * memoryLeak.cc
 *
 *  Created on: 2011-4-21
 *      Author: FanOpi
 */

#include <stdlib.h>

#include "memoryLeakTrace.h"
#include "../../library/string.h"

namespace YF{

/// class MemoryLeakTrace ///
MemoryLeakTrace* MemoryLeakTrace::s_singleton_ = NULL;

MemoryLeakTrace::MemoryLeakTrace(){
	for(int i = 0; i<RECORD_SIZE; i++){
		mallocRecord_[i] = NULL;
	  newRecord_[i] = NULL;
	}
}

MemoryLeakTrace::~MemoryLeakTrace(){
	for(int i = 0; i<RECORD_SIZE; i++){
		 if (NULL != mallocRecord_[i]){
			 MemTraceEntry* p_ernty = mallocRecord_[i];
			 if(NULL != p_ernty->file){
				 ::free(p_ernty->file);
			 }
			 ::free(p_ernty);
			 mallocRecord_[i] = NULL;
		 }
	}

	for(int i = 0; i<RECORD_SIZE; i++){
		 if (NULL != newRecord_[i]){
			 MemTraceEntry* p_ernty = newRecord_[i];
			 if(NULL != p_ernty->file){
				 ::free(p_ernty->file);
			 }
			 ::free(p_ernty);
			 newRecord_[i] = NULL;
		 }
	}
}

MemoryLeakTrace* MemoryLeakTrace::GetMemoryLeakTrace(){
	if (NULL == s_singleton_){
		s_singleton_= new MemoryLeakTrace;
	}
		return s_singleton_;
}

void MemoryLeakTrace::Trace(TraceType type, void* address, size_t size, const YFCHAR* file, int line){
	MemTraceEntry* p_ernty = (MemTraceEntry*)::malloc(sizeof(MemTraceEntry));
	p_ernty->serialnumber = serial_++;
	p_ernty->address = address;
	p_ernty->size = size;
	p_ernty->file = (YFCHAR*)::malloc(sizeof(YFCHAR) * (StrlenUni(file)+1));
	StrcpyUni(p_ernty->file, file);
	p_ernty->line = line;

	switch(type){
		case MEMORY_FREE:{
			PutInRecord(mallocRecord_, p_ernty);
			break;
		}
		case MEMORY_DELETE:{
			PutInRecord(newRecord_, p_ernty);
			break;
		}
		default:
			break;
	}
}

size_t MemoryLeakTrace::Detrace(TraceType type, void* address){
	MemTraceEntry** trace_record = NULL;
	switch(type){
		case MEMORY_MALLOC:{
			trace_record =	mallocRecord_;
			break;
		}
		case MEMORY_NEW: {
			trace_record =	newRecord_;
			break;
		}
		default:
			break;
	}

	MemTraceEntry* p_ernty = GetRecord(trace_record, address);
	size_t size = p_ernty->size;
	CancelRecord(trace_record, p_ernty);
  return size;
}

void MemoryLeakTrace::Report(const YFCHAR* output_file_name){

}

void MemoryLeakTrace::PutInRecord(MemTraceEntry* trace_record[], MemTraceEntry* p_ernty){
  for(int i = 0; i < RECORD_SIZE; i++){
  	if (NULL == trace_record[i])
  		trace_record[i] = p_ernty;
  	}
}

MemTraceEntry* MemoryLeakTrace::GetRecord(MemTraceEntry* trace_record[], void* p_address){
  for(int i = 0; i < RECORD_SIZE; i++){
  	if (NULL != trace_record[i] && trace_record[i]->address == p_address)
  		return trace_record[i];
  }
  return NULL;
}

void MemoryLeakTrace::CancelRecord(MemTraceEntry* trace_record[], MemTraceEntry* p_ernty){
  for(int i = 0; i < RECORD_SIZE; i++){
  	if (trace_record[i] == p_ernty)
  		 trace_record[i] = NULL;
  }
}

} /// namespace YF
