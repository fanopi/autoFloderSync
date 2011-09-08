/*
 * messageObject.h
 *
 *  Created on: 2011-4-13
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_
#define FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_

#include "../../support/typedef.h"
#include "../../support/classsupport.hpp"
#include "../../operator/operatecode.h"

namespace yf{

typedef enum _ThreadType{
	GOVERNOR,
	TREEBUILDER,
	FOLDER,
	TXT,
	EPUB,
	PDF,
	UNKNOWN,
}ThreadType;

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

  MessageObj(OperateCode operator_code,
		  	  ThreadType receiver_type,
		  	  unsigned int receiver_id,
		  	  ThreadType send_type,
		  	  unsigned int send_id){
  	operator_code_	= operator_code;
  	receiver_type_ 	= receiver_type;
  	send_type_		= send_type;
    send_id_ 		= send_id;
    receiver_id_ 	= receiver_id;

	message_sequence_ = ++class_message_sequence_;
  }

  Status GetParamter(long* param1, long* param2, long* param3, long* param4){
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

  	return OK;
EXIT:
	return FAIL;
  }

  void SetParamter(long param1, long param2=0, long param3=0, long param4=0){
  		param1_ = param1;
  		param2_ = param2;
  		param3_ = param3;
  		param4_ = param4;
  }

  void SetOpCode(OperateCode code){
  	operator_code_ = code;
  }

  OperateCode GetOpCode(){
  	return operator_code_;
  }

  ThreadType GetReceiverType(){
  	return receiver_type_;
  }
  // todo
  void SetReceiverType(){

  }
  // todo
  unsigned int GetReceiverId(){
	  return 0;
  }
  // todo
  void SetReceiverId(){

  }
  // todo
  unsigned int GetSendId(){
	  return 0;
  }
  // todo
  void SetSendId(){

  }

 private:
  static long	class_message_sequence_;
  long			message_sequence_; // the sequence of the message package.

  ThreadType	receiver_type_;
  unsigned int	receiver_id_;
  ThreadType	send_type_;
  unsigned int	send_id_;
  OperateCode 	operator_code_;
  long		param1_;
  long		param2_;
  long		param3_;
  long		param4_;
 // DISALLOW_COPY_ADN_ASSIGN(MessageObj);
};

}/// namespace YF
#endif /* FPJ_SOURCE_TABLEMANAGE_SMALLOBJECT_MESSAGEOBJECT_H_ */
