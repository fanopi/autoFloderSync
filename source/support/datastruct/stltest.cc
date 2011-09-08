/*
 * stltest.cc
 *
 *  Created on: 2011-4-23
 *      Author: FanOpi
 */

#include "vector.hpp"

namespace YF{

class TestClass{
	public:
		TestClass();
		~TestClass();
};

void UnitTest(void){
	YF::Vector<TestClass> vTestClass;
}

} /// namespace YF

