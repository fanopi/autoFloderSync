/*
 * testrunnable.cc
 *
 *  Created on: 2011-9-8
 *      Author: fan
 */
#include "runnable.h"

/* construct a class that using for test the Runnable interface
 * nothing to do, the class should have ability running
 * in mutilthreads.*/
class TestRunnable : Runnable{
};

int main(){
	int result = 0;
	TestRunnable test_class;
	Message mes1,mes2,mes3,mes4;
	result = SendMessage(test_class, mes1);
	result = SendMessage(test_class, mes2);
	result = SendMessage(test_class, mes3);
	result = SendMessage(test_class, mes4);

	result = test_class.Run();

	return result;
}
