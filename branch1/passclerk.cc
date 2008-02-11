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

		// clerk checks customer got application and picture complete
		if( (c->checkApplication() == true) && (c->checkPicture() == true))
		{
			// both application and picture are complete, the customer complete passport
			// the clerk takes 100 ticks to file the passport
			currentThread->Yield();
			c->completePassport();
			printf("[PassClerk]%s%d has complete customer %d passport.................\n",clerkName,clerkID,c->getID());
		}
		else
		{
			// application and picture are not complete.
			// the clerk ask the customer to do something???
		}
	}
	~PassClerk(){
	}
			
};
#endif


