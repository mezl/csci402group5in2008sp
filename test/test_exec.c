/* 
test for exec system call
*/


#include "syscall.h"
#include "string.h"


int main()
{

  println("[Exec main]Main! start");

  println("Exec the 1st fork test file");
	Exec("test_fork");

  println("Exec the 2nd fork test file");
	Exec("test_fork");
  println("[Exec main]main exits");

  Exit(0);
}


