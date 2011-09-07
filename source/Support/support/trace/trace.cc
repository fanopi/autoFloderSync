/*
 * trace.cc
 *
 *  Created on: 2011-4-28
 *      Author: FanOpi
 */

FUNSTATE trace::trace(const YFCHAR* output_file_name){
	FILE* file_handle;
	if (NULL != output_file_name)
		file_handle = FopenUni(output_file_name, "a");

	SharedString* p_string_build = SharedString::GetSharedString();
	while(!scan_result_queue_.IsQueueEmpty()){
		int index = scan_result_queue_.OutQueue();
		const YFCHAR* p_file_name =	p_string_build->GetString(index);
		FputsUni(p_file_name, file_handle);
		const YFCHAR* p_line = "\n";
		FputsUni(p_line, file_handle);
	}
	return RTN_OK;
}
