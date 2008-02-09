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
		if (c->checkPassport == true)
		{
			// passport is complete, charge 100 dollars and customer complete cashier
			c->completeCashier();
			c->chargeMoney(100);
			t->addMoney(100);
		}
		else
		{
			// passport is not complete, ask the customer to go to back of the cashier line
			c->gotoCashierLine();
		}
	}
	~CashClerk(){
	}
			
};
#endif


