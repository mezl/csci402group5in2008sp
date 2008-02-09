#ifndef PICCLERK_C_DEFINED
#define PICCLERK_C_DEFINED
#include "clerk.h"

class PicClerk:public Clerk
{
	public:
	PicClerk(cLine *l,cTable *t,int id,char *name):
		Clerk(l,t,id,name)
	{
		
	}
	void handleCustomer(Customer *c){	
		printf("[PicClerk]%s%d is handling customer %d..................\n",clerkName,clerkID,c->getID());
		c->completePicture();
	}
	~PicClerk(){
	}
			
};
#endif


