/* 
test for exec system call
*/


#include "syscall.h"
#include "string.h"


int main()
{

  println("[Exec main]Main! start");

  println("Exec the 1st matmult file");
	Exec("matmult");

  println("Exec the 2nd matmult file");
	Exec("matmult");
  println("[Exec main]main exits");

  Exit(0);
}


