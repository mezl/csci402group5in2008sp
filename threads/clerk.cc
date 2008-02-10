//Base clerk function
#include "clerk.h"
Clerk::Clerk(cLine *l,cTable *t,int id,char *name)
{
	cline = l;
	table = t;//share with manager
	clerkID = id;
	clerkName = name;
	char msg[20];
	sprintf(msg,"%s%d",name,id)
	clerkLock = new Lock(msg);
	clerkCondition = new Condition(msg);
}
void Clerk::run()
{
	printf("[Clerk]%s %d Start Running \n",clerkName,clerkID);
	while(1)
	{
		//Do Lock
		bool serviced= false;

		cline->preferAcquire(clerkName,clerkID);		
		
		printf("[Clerk]%s %d Get both line lock\n",clerkName,clerkID);
		if(!cline->IsPreferLineEmpty()) //if there is customer in prefer line
		{
			serviced= true;
			printf("[Clerk]%s %d check prefer line %d\n",clerkName,clerkID,cline->getID());
			Customer *c = (Customer *)cline->getNextPreferLineCustomer(clerkName,clerkID);
			c->setClerk(clerkLock,clerkCondition);//tell this customer is served by me	
			clerkLock->Acquire();
			handleCustomer(c);
			c->wakeup(ckerkLock);
			clerkLock->Release();
		}
		cline->preferRelease(clerkName,clerkID);

		if(!serviced)
		{
			cline->regAcquire(clerkName,clerkID);		
			if(!cline->IsRegLineEmpty())
			{
				serviced = true;
				printf("[Clerk]%s %d check reg line %d\n",clerkName,clerkID,cline->getID());
				Customer *c =(Customer *)cline->getNextRegLineCustomer(clerkName,clerkID);
				c->setClerk(clerkLock,clerkCondition);//tell this customer is served by me	
				clerkLock->Acquire();
				handleCustomer(c);
				c->wakeup(ckerkLock);
				clerkLock->Release();
			}
			cline->regRelease(clerkName,clerkID);		
		}
		if(!serviced)
		{
			//Go Sleep
			printf("[Clerk]%s %d have no customer in line %d,leave table\n",clerkName,clerkID,cline->getID());
			table->acquireLock(clerkName,clerkID);
			table->leaveTable(clerkName,clerkID);
			table->releaseLock(clerkName,clerkID);
			printf("[Clerk]%s %d now come back to table \n",clerkName,clerkID);
		}
	}//while(1);
}


