//cashclerk.cc

#ifndef CASHCLERK_C_DEFINED
#define CASHCLERK_C_DEFINED
#include "clerk.h"

class CashClerk:public Clerk
{
	public:
	CashClerk(cLine *l,cTable *t,int id,char *name):
		Clerk(l,t,id,name)
	{
		
	}
	void handleCustomer(Customer *c){	
		printf("[CashClerk]%s%d is handling customer %d.................\n",clerkName,clerkID,c->getID());
			
		// check customer if passport is complete
		if (c->checkPassport() == true)
		{
			// passport is complete, charge 100 dollars and customer complete cashier
			c->completeCashier();
			c->chargeMoney(100);
			table->addMoney(100);
			printf("[CashClerk]%s%d has complete customer %d cashier.................\n",clerkName,clerkID,c->getID());
		}
		else
		{
			// passport is not complete
			// customer gets punished to wait for a certain amount of time 
			// then ask the customer to go to back of the cashier line
			c->punished();
			c->gotoCashierLine();
		}
	}
	~CashClerk(){
	}
			
};
#endif


