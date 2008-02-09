//passclerk.cc

#ifndef PASSCLERK_C_DEFINED
#define PASSCLERK_C_DEFINED
#include "clerk.h"

class PassClerk:public Clerk
{
	public:
	PassClerk(cLine *l,cTable *t,int id,char *name):
		Clerk(l,t,id,name)
	{
		
	}
	void handleCustomer(Customer *c){	
		printf("[PassClerk]%s%d is handling customer %d.................\n",clerkName,clerkID,c->getID());


		c->completePassport();
	}
	~PassClerk(){
	}
			
};
#endif


