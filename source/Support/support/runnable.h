/*
 * runnable.h
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_
#define FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_

#include <stdio.h>
#include <Windows.h>

#include "../typedef.h"
#include "datastruct/queue.hpp"
#include "../../tablemanager/smallobject/messageobject.hpp"
#include "../os/threads/threads.hpp"

namespace YF{

class Runnable{
 public:
	virtual bool Run() = 0;
	virtual int Task(long paramter) = 0;
	Runnable(){
	}

	virtual	~Runnable(){
	}
};

class MessageQueue{
 public:
	MessageQueue(){
		h_product_Semaphore = CreateSemaphore(NULL,	1, 1, NULL);
		h_consume_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);
		h_product_Queue_Semaphore = CreateSemaphore(NULL, Max_QUEUE_SIZE, Max_QUEUE_SIZE, NULL);
		h_consume_Queue_Semaphore = CreateSemaphore(NULL, 0, Max_QUEUE_SIZE, NULL);
	}
	FUNSTATE  InMessageQueue(MessageObj* p_message);
	MessageObj* GetMessageFormQueue();

 private:
	  const static int Max_QUEUE_SIZE = (40*1024 - 100);
		Queue<MessageObj*>  message_queue_;

	  HANDLE h_product_Semaphore;
	  HANDLE h_consume_Semaphore;
	  HANDLE h_product_Queue_Semaphore;
	  HANDLE h_consume_Queue_Semaphore;
};

} /// namespace YF
#endif /* FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_ */
