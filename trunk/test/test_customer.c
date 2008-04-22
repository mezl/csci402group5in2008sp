/* 
test lock and condition RPC
*/


#define NETWORK
#include "syscall.h"
#include "string.h"

void TestCustomerAcquire()
{
   int ip,port;
   ip = GetMachineID();
   port = GetMailBox();
   print("Machine ID is ");
   printi(ip);
   print(" port is ");
   printi(port);
   print("\n");
   
   CustomerAcquire(ip,port,0);
   print("Customer ");
   printi(ip);
   print(":");
   printi(port);
   print(" Finish Appilcation\n");
   
   CustomerAcquire(ip,port,1);
   print("Customer ");
   printi(ip);
   print(":");
   printi(port);
   print(" Finish Picture\n");
   
   CustomerAcquire(ip,port,2);
   print("Customer ");
   printi(ip);
   print(":");
   printi(port);
   print(" Finish Passport\n");
   
   CustomerAcquire(ip,port,3);
   print("Customer ");
   printi(ip);
   print(":");
   printi(port);
   print(" Finish Cashier\n");
   
   print("======== Customer ");
   printi(ip);
   print(":");
   printi(port);
   print(" Finish Everything===========\n");
}
int main()
{
      TestCustomerAcquire();
	Exit(0);
}


