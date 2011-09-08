/*
 * autoPtr.cc
 *
 *  Created on: 2011-4-18
 *      Author: Administrator
 */

#include"scopepointer.h"
#include <stdio.h>
#include "stdlib.h"
#include "../library/stdlib.h"
#include "datastruct/stlsupport.hpp"

namespace YF{

class TestMe{
 public:
	explicit TestMe(int i): i_(i){
			printf("TestMe init %d\n",this->i_);
		}

	~TestMe(){
		printf("TestMe Deinit %d\n",i_);
	}

	int I_() const{
		return i_;
	}

 private:
	int i_;
};

} /// namespace YF
