/*
 * classLimit.hpp
 * $$ some macro defination using for global class.
 *  Created on: 2011-4-13
 *      Author: Administrator
 */

#ifndef FPJ_SOURCE_INCLUDE_CLASSLIMIT_HPP_
#define FPJ_SOURCE_INCLUDE_CLASSLIMIT_HPP_

#define DISALLOW_COPY_ADN_ASSIGN(TypeName)\
	TypeName(const TypeName&);\
	void operator=(TypeName);

#endif /* FPJ_SOURCE_INCLUDE_CLASSLIMIT_HPP_ */
