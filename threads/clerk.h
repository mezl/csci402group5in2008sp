#ifndef  CLERK_H_DEFINED
#define CLERK_H_DEFINED
#include "cline.h"
#include "table.h"
#include "customer.h"

class Clerk	
{
	public:
	Clerk(cLine *l,Table *t,int id);
	virtual ~Clerk();
	void run();
	virtual void handleCustomer(Customer *c){
	return NULL;
	
	};
	private:
	cLine *cline;
	Table *table;
	int clerkID;
};
#endif


