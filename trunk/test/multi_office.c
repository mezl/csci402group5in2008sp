/* 
test for multiple office
*/


#include "syscall.h"
#include "string.h"


int main()
{

  println("[Exec main]Main! start");

  println("Exec the 1st passport file");
	Exec("project2_final");
	
	println("Exec the 2nd passport file");
	Exec("project2_final");

	println("Exec the 3rd passport file");
	Exec("project2_final");

  println("[Exec main]main exits");

  Exit(0);
}
