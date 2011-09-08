/*
 * init.cpp
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#include "init.h"
#include "govern.h"
#include "../include/typedef.h"
#include "../operator/operatorcode.h"
#include "../tablemanager/smallobject/sharedstring.h"

namespace YF{

FUNSTATE Init(const YFCHAR* p_start_scan_path){
	FUNSTATE state = RTN_FAIL;

	SharedString* p_string_build = SharedString::GetSharedString();
	if (NULL == p_string_build)
		goto ERROR_EXIT;
	StringTrait trait = p_string_build->StoreString(p_start_scan_path);
	long string_index = p_string_build->GetStringIndex(trait);
	if (SharedString::INVALID_STRING_INDEX == string_index)
		goto ERROR_EXIT;

	MessageObj* p_first_message = new MessageObj(OP_CODE_FOLDER_ENUM, FOLDER);
	if (NULL == p_first_message)
		goto ERROR_EXIT;
	long recruse_scan = 1;
	p_first_message->SetParamter(string_index, recruse_scan);

	Governor* pGovernor = Governor::GetGovernor();
	if (NULL == pGovernor)
		goto ERROR_EXIT;
	state = pGovernor->InMessageQueue(p_first_message);
	bool thread_fail = pGovernor->Run();
	if (true == thread_fail)
		goto ERROR_EXIT;

	state = RTN_OK;

	Sleep(100 * 1000);

ERROR_EXIT:
	return state;
}

static void UnitTest(void)
{
	const YFCHAR* p_start_scan_path = "e:\\×ÊÁÏ";
	Init(p_start_scan_path);
}

}/// namespace YF

void main(void){
	YF::UnitTest();
}
