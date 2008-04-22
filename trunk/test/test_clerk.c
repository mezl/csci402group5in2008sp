/* 
test lock and condition RPC
*/


#define NETWORK
#include "syscall.h"
#include "string.h"



void TestClerkReg()
{
   int ip,port;
   ip = GetMachineID();
   port = GetMailBox();
   printf("Machine ID is %d port is %d\n",ip,port);
   while(1){   
      int customer = ClerkReg(ip,port,0);
      printf("[UserClerk]Finish handle customer %d %d\n",customer/10,customer%10);

   }   
}
int main()
{
      TestClerkReg();

	Exit(0);
}


