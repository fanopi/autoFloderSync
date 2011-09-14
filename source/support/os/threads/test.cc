/*
 * test.cc
 *
 *  Created on: 2011-9-14
 *      Author: fanopi
 */

#include "threads.hpp"
#include "stdio.h"
namespace yf{

class TestMutilThread {
 public:
    int foo(long parameter);
    void run();
 private:
   YFThread thread_;
};

void TestMutilThread::run(){
  thread_.Start(this, (NullThreadFunction)&foo, 0);
}

int foo(long parameter){
  printf( "hello world\n");
  return 0;
}

int main(){
  TestMutilThread test;
  test.run();

  return 0;
}


}
