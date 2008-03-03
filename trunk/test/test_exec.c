/* 
test for exec system call
*/


#include "syscall.h"
#include "string.h"


int main()
{

  println("[Exec main]Main! start");

  println("Exec the fork test file");
	Exec("test_fork");

  println("[Exec main]main exits");

  Exit(0);
}


