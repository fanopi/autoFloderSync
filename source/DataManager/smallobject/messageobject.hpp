/*
 * messageObject.h
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_
#define FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_

#include "../../include/typedef.h"
#include "../../include/support/classsupport.hpp"
#include "../../operator/operatorcode.h"

namespace YF{

enum Thread_type{
	GOVERNOR,
	TREEBUILDER,
	FOLDER,
	TXT,
	EPUB,
	PDF,
	UNKNOWN,
};

class MessageObj{
 public:
	typedef long StringIndex;
  MessageObj(){
  	receiver_type_ = UNKNOWN;
  	send_type_ = UNKNOWN;
    param1_ = NULL;
		param2_ = NULL;
		param3_ = NULL;
		param4_ = NULL;
		message_sequence_ = ++class_message_sequence_;
  }

  MessageObj(OperatorCode operator_code, Thread_type receiver_type, unsigned int receiver_id=0, Thread_type send_type=UNKNOWN, unsigned int	send_id=0){
  	operator_code_	= operator_code;
  	receiver_type_ 	= receiver_type;
  	send_type_ 			= send_type;
    send_id_ 				= send_id;
    receiver_id_ 		= receiver_id;
		message_sequence_ = ++class_message_sequence_;
  }

  FUNSTATE GetParamter(long* param1, long* param2=NULL, long* param3=NULL, long* param4=NULL){
  	if (NULL == param1)
  		goto EXIT;
  	else
  		*param1 = param1_;
  	if (NULL != param2)
  		*param2 = param2_;
  	if (NULL != param3)
  		*param3 = param3_;
  	if (NULL != param4)
  		*param4 = param4_;

  	return RTN_OK;
	EXIT:
		return RTN_FAIL;
  }

  void SetParamter(long param1, long param2=0, long param3=0, long param4=0){
  		param1_ = param1;
  		param2_ = param2;
  		param3_ = param3;
  		param4_ = param4;
  }

  void SetOpCode(OperatorCode code){
  	operator_code_ = code;
  }

  OperatorCode GetOpCode(){
  	return operator_code_;
  }

  Thread_type GetReceiverType(){
  	return receiver_type_;
  }

  void SetReceiverType(){

  }

  unsigned int GetReceiverId(){

  }

  void SetReceiverId(){

  }

  unsigned int GetSendId(){

  }

  void SetSendId(){

  }

 private:
  static long		class_message_sequence_;
  long					message_sequence_; // the sequence of the message package.

  Thread_type		receiver_type_;
  unsigned int	receiver_id_;
  Thread_type		send_type_;
  unsigned int	send_id_;
  OperatorCode 	operator_code_;
  long					param1_;
  long					param2_;
  long					param3_;
  long					param4_;
  DISALLOW_COPY_ADN_ASSIGN(MessageObj);
};

}/// namespace YF
#endif /* FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_ */
