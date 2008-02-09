//Base clerk function
#include "clerk.h"
Clerk::Clerk(cLine *l,Table *t)
{
	cline = l;
	table = t;//share with manager
	
}
void Clerk::run()
{
	while(1)
	{
		//Do Lock
		cline->Acquire();
		if(!cline->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			Customer *c = (Customer *c)cline->getNextPreferLineCustomer();
			handleCustomer(c);
		}
		else if(!cline->IsRegLineEmpty())
		{
			Customer *c =(Customer *c)cline->getNextRegLineCustomer();
			handleCustomer(c);
		}else{
			//Go Sleep
			cline->Release();			
			table->leaveTable();
		}	
		cline->Release();
	}
}

