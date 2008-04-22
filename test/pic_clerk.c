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
   printf("Picture Clerk Machine ID is %d port is %d\n",ip,port);
   while(1){   
      int customer = ClerkReg(ip,port,1);
      printf("[PicClerk]Picture Clerk Finish handle customer %d %d\n",customer/10,customer%10);

   }   
}
int main()
{
      TestClerkReg();

	Exit(0);
}


