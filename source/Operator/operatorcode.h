/*
 * operatorCode.h
 * $$$ define all the operator acceptial code
 *
 *  Created on: 2011-4-12
 *      Author: FanOpi
 */

#ifndef FPJ_SOURCE_OPERATOR_OPERATORCODE_H_
#define FPJ_SOURCE_OPERATOR_OPERATORCODE_H_
namespace YF{

// folderOp code
typedef enum _OperatorCode{
	/* govern */
	OP_CODE_STOP,
	OP_CODE_FOLDER_ENUM_DOING,
	OP_CODE_FOLDER_ENUM_DONE,
	OP_CODE_BUILD_TREE_DONE,

	/* folder */
	OP_CODE_FOLDER_ENUM,
	OP_CODE_FOLDER_REPORT,
	OP_CODE_FOLDER_RETURN_RESULT,

	/* treebuilder */
	OP_CODE_INSERT_NODE,
	OP_CODE_OUTPUT,
	OP_CODE_BUILD_TREE,
	OP_CODE_COMPAIR_TREE,
}OperatorCode;

} /// namespace YF
#endif /* FPJ_SOURCE_OPERATOR_OPERATORCODE_H_ */
