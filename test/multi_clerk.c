/* 
test lock and condition RPC
*/


#define NETWORK
#include "syscall.h"
#include "string.h"




int main()
{
   Exec("../test/app_clerk");
   Exec("../test/pic_clerk");
   Exec("../test/pass_clerk");
   Exec("../test/cash_clerk");
	Exit(0);
}


