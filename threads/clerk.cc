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
	while(1)
	{
		//Do Lock
		cline->Acquire(clerkName,clerkID);
		if(!cline->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			printf("%s %d check prefer line %d",clerkName,clerkID,cline->getID());
			Customer *c = (Customer *)cline->getNextPreferLineCustomer(clerkID);
			handleCustomer(c);
		}
		else if(!cline->IsRegLineEmpty())
		{
			printf("%s %d check reg line %d",clerkName,clerkID,cline->getID());
			Customer *c =(Customer *)cline->getNextRegLineCustomer(clerkID);
			handleCustomer(c);
		}else{
			//Go Sleep
			printf("%s %d have no customer in line %d,leave table",clerkName,clerkID,cline->getID());
			cline->Release(clerkName,clerkID);			
			table->leaveTable();
		}	
		cline->Release(clerkName,clerkID);
	}
}


