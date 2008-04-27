/* 
test lock and condition RPC
*/


#define NETWORK
#include "syscall.h"
#include "string.h"



void TestManager()
{
   int ip,money = 0;;
   ip = GetMachineID();
   ManagerReg(ip);
   print("[Manager]Total money manager receive ");
   printi(money);
   print("\n");
   while(1){   
      int amount = ManagerGetMoney(ip);
      print("[Manager]Manager receive ");
      printi(amount);
      print("from server\n");
      money += amount;
      print("[Manager]Total money manager receive <<<< ");
      printi(money);
      print(" >>>>\n");

      

   }   
}
int main()
{
      TestManager();

	Exit(0);
}


