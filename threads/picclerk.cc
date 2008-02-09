#ifndef PICCLERK_C_DEFINED
#define PICCLERK_C_DEFINED
#include "clerk.h"
#include <time.h>

class PicClerk:public Clerk
{
	public:
	PicClerk(cLine *l,cTable *t,int id,char *name):
		Clerk(l,t,id,name)
	{
		//random function initilization
		srand(time(NULL));
	}
	void handleCustomer(Customer *c){	
		printf("[PicClerk]%s%d is handling customer %d..................\n",clerkName,clerkID,c->getID());
		
		// there is a 25% of customer not liking the picture
		// retake the picture until the customer is satisfied
		while(rand()%4 == 0)
		{
			printf("[PicClerk]%s%d is retaking picture for customer %d..................\n",clerkName,clerkID,c->getID());
		}
		printf("[PicClerk]%s%d has complete customer %d picture..................\n",clerkName,clerkID,c->getID());
		c->completePicture();
	}
	~PicClerk(){
	}
			
};
#endif


