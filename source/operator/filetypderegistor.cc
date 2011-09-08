/*
 * registor.cpp
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#include "../include/typedef.h"
#include "filetyperegistor.h"
#include "fileoperator.h"
#include "folder/folderoperator.h"
#include "../tablemanager/smallobject/sharedstring.h"
#include "../include/os/file/file.h"
#include "../include/support/utility.h"

namespace YF{

FileRegister::FileRegister(){

}

FileOperator* FileRegister::RegisterFile(long file_name_string_index){
	SharedString* p_string_build = SharedString::GetSharedString();
	const YFCHAR* p_file_name = p_string_build->GetString(file_name_string_index);
	FileTypes file_type = GetFileType(p_file_name);
	switch (file_type){
		case 	FT_FOLD:
			return FloderOperator::GetFloderOperator();
		case	FT_TXT:
		case	FT_EPB:
		case	FT_PDF:
		return NULL;
	}
return NULL;
}

} /// namespace YF
