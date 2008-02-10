#ifndef  CLERK_H_DEFINED
#define CLERK_H_DEFINED
#include "cline.h"
#include "ctable.h"
#include "thread.h"
#include "customer.h"

class Clerk	
{
	public:
	Clerk(cLine *l,cTable *t,int id,char *name);
	virtual ~Clerk(){}
	void run();
	virtual void handleCustomer(Customer *c){
	
	
	}
	int getID(){return clerkID;}
	char *getName(){return clerkName;}
	protected:
	cLine *cline;
	cTable *table;
	int clerkID;
	char *clerkName;
	Lock *clerkLock;
	Condition *clerkCondition;
};
#endif


