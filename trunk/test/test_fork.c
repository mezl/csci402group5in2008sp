/* 
test for fork system calls 
main shall first fork func1 and func2 than print message 
func1 and func2 both print message and exit 
*/


#include "syscall.h"
#include "string.h"


void func1(){
  println("[func1] start");

  println("[func1] exit");
  Exit(0);
}

void func2(){

  println("[func2] start");

  println("[func2] exits");
  Exit(0);
}

int main()
{

  println("[main]Main! start");

  println("[main]func1 forked by main");
  Fork(func1);

  println("[main]func2 forked by main");
  Fork(func2);

  println("[main]main exits");

  Exit(0);
}


