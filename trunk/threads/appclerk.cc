#ifndef  APPCLERK_H_DEFINED
#define APPCLERK_H_DEFINED
#include "clerk.h"

class AppClerk:public Clerk
{
	public:
	AppClerk(cLine *l,cTable *t,int id,char *name):
		Clerk(l,t,id,name)
	{
		
	}
	void handleCustomer(Customer *c){	
		printf("[AppClerk]%s%d is handling customer %d\n",clerkName,clerkID,c->getID())
		c->completeApplication();
	}
	~AppClerk(){
	}
			
};
#endif


