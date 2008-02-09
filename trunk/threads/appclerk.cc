#ifndef  APPCLERK_H_DEFINED
#define APPCLERK_H_DEFINED
#include "clerk.h"

class AppClerk:public Clerk
{
	public:
	AppClerk(cLine *l,cTable *t,int id):
		Clerk(l,t,id)
	{
		
	}
	void handleCustomer(Customer *c){	
		c->completeApplication();
	}
	~AppClerk(){
		~Clerk();
	}
			
};
#endif


