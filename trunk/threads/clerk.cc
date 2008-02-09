//Base clerk function
#include "clerk.h"
Clerk::Clerk(cLine *l,cTable *t,int id)
{
	cline = l;
	table = t;//share with manager
	clerkID = id;
}
void Clerk::run()
{
	while(1)
	{
		//Do Lock
		cline->Acquire();
		if(!cline->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			Customer *c = (Customer *)cline->getNextPreferLineCustomer(clerkID);
			handleCustomer(c);
		}
		else if(!cline->IsRegLineEmpty())
		{
			Customer *c =(Customer *)cline->getNextRegLineCustomer(clerkID);
			handleCustomer(c);
		}else{
			//Go Sleep
			cline->Release();			
			table->leaveTable();
		}	
		cline->Release();
	}
}


