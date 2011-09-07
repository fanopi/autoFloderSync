/*
 * runnable.cpp
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#include "runnable.h"

namespace YF{

FUNSTATE MessageQueue::InMessageQueue(MessageObj* p_first_message){
  BOOL   bContinue = TRUE;
  while(bContinue){
  	if (WAIT_OBJECT_0 != WaitForSingleObject(h_product_Semaphore, 0L))
  			continue;
		if ( WAIT_OBJECT_0 != WaitForSingleObject(h_product_Queue_Semaphore, 0L)){
			 if (!ReleaseSemaphore(h_product_Semaphore, 1, NULL))
				 goto EXIT;
			continue;
		}

	 this->message_queue_.InQueue(p_first_message);

	 if (!ReleaseSemaphore(h_consume_Queue_Semaphore, 1, NULL))
		 goto EXIT;
	 if (!ReleaseSemaphore(h_product_Semaphore, 1, NULL))
		 goto EXIT;

	 bContinue = FALSE;
  }

  return RTN_OK;
EXIT:
  return RTN_FAIL;
}

MessageObj* MessageQueue::GetMessageFormQueue(){
	MessageObj* p_message;
  BOOL   bContinue = TRUE;

  while(bContinue){
		if (WAIT_OBJECT_0 != WaitForSingleObject(h_consume_Semaphore, 0L))
				continue;
		if (WAIT_OBJECT_0 !=  WaitForSingleObject(h_consume_Queue_Semaphore, 0L)){
			if (!ReleaseSemaphore(h_consume_Semaphore, 1, NULL))
				goto EXIT;
			continue;
		}

	  p_message = this->message_queue_.OutQueue();

		if (!ReleaseSemaphore(h_product_Queue_Semaphore, 1, NULL))
			goto EXIT;
		if (!ReleaseSemaphore(h_consume_Semaphore, 1, NULL))
			goto EXIT;

		bContinue = FALSE;
  }

	return p_message;
EXIT:
	return NULL;
}

} /// namespace YF ///
