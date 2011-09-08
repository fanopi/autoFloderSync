/*
 * runnable.h
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_
#define FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_

#include <stdio.h>

#include "../typedef.h"
#include "datastruct/queue.hpp"
#include "../../datamanager/smallobject/messageobject.hpp"
#include "../os/threads/threads.hpp"

namespace yf{

class MessageQueue{
 public:
	MessageQueue(){
		productor_Semaphore = CreateSemaphore(NULL,	1, 1, NULL);
		consumer_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);
		productor_Queue_Semaphore = CreateSemaphore(NULL, kMaxQUEUESIZE, kMaxQUEUESIZE, NULL);
		consumer_Queue_Semaphore = CreateSemaphore(NULL, 0, kMaxQUEUESIZE, NULL);
	}
	Status  InQueue(MessageObj* message);
	MessageObj* OutQueue();

 private:
  const static int kMaxQUEUESIZE = (40*1024 - 100);
  Queue<MessageObj*>  message_queue_;

  Handle productor_Semaphore;
  Handle consumer_Semaphore;
  Handle productor_Queue_Semaphore;
  Handle consumer_Queue_Semaphore;
};


class Runnable{
 public:
  virtual bool Run() = 0;
  virtual int Task(long paramter) = 0;
  Runnable(){
  }
  virtual ~Runnable(){
  }
 private:
  int state_;
  MessageQueue queue_;
};

} /// namespace YF
#endif /* FPJ_SOURCE_INCLUDE_SUPPORT_RUNNABLE_H_ */
