#ifndef  APPCLERK_H_DEFINED
#define APPCLERK_H_DEFINED
#include "clerk.h"

class AppClerk:public Clerk
{
	public:
	AppClerk(cLine *l,Table *t,int id);
	void handleCustomer(Customer *c){	
		c->completeApplication();
	}
	~AppClerk();
			
};
#endif


