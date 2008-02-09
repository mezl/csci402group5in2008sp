#ifndef  CLERK_H_DEFINED
#define CLERK_H_DEFINED
#include "cline.h"
#include "ctable.h"
#include "customer.h"

class Clerk	
{
	public:
	Clerk(cLine *l,cTable *t,int id);
	virtual ~Clerk(){}
	void run();
	virtual void handleCustomer(Customer *c){
	
	
	}
	int getID(){return clerkID;}
	private:
	cLine *cline;
	cTable *table;
	int clerkID;
};
#endif


