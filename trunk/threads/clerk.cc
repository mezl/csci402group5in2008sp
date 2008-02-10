//Base clerk function
#include "clerk.h"
Clerk::Clerk(cLine *l,cTable *t,int id,char *name)
{
	cline = l;
	table = t;//share with manager
	clerkID = id;
	clerkName = name;
}
void Clerk::run()
{
	printf("[Clerk]%s %d Start Running \n",clerkName,clerkID);
	while(1)
	{
		//Do Lock
		cline->Acquire(clerkName,clerkID);
		printf("[Clerk]%s %d Get both line lock\n",clerkName,clerkID);
		if(!cline->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			printf("[Clerk]%s %d check prefer line %d\n",clerkName,clerkID,cline->getID());
			Customer *c = (Customer *)cline->getNextPreferLineCustomer(clerkName,clerkID);
			handleCustomer(c);
			cline->Release(clerkName,clerkID);
		}
		else if(!cline->IsRegLineEmpty())
		{
			printf("[Clerk]%s %d check reg line %d\n",clerkName,clerkID,cline->getID());
			Customer *c =(Customer *)cline->getNextRegLineCustomer(clerkName,clerkID);
			handleCustomer(c);
			cline->Release(clerkName,clerkID);
		}
		else{
			//Go Sleep
			printf("[Clerk]%s %d have no customer in line %d,leave table\n",clerkName,clerkID,cline->getID());
			cline->Release(clerkName,clerkID);			
			table->acquireLock(clerkName,clerkID);
			table->leaveTable(clerkName,clerkID);
			table->releaseLock(clerkName,clerkID);
			printf("[Clerk]%s %d now come back to table \n",clerkName,clerkID);
		}
	}
}


