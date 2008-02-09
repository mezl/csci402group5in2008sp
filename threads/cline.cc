#include "cline.h"
cLine::cLine()
{

}
cLine::cLine(int ID)
{
	preferLineCond = new Condition("Customer Prefer cLine Cond") ;	
	preferLineLock= new Lock("Customer Prefer cLine Lock") ;	
	preferLineQueue = new List;
	regLineCond = new Condition("Customer Prefer cLine Cond") ;	
	regLineLock= new Lock("Customer Prefer cLine Lock") ;	
	regLineQueue = new List;

	preferLineCount = 0;
	regLineCount = 0;
	lineID = ID;
	amount = 0;
}
cLine::~cLine()
{

}
void cLine::preferAcquire()
{
	printf("Prefer Line %d is acquired lock by \n",lineID);
	preferLineLock->Acquire();
}
void cLine::preferRelease()
{
	printf("Prefer Line %d is release lock by \n",lineID);
	preferLineLock->Release();
}

void cLine::regAcquire()
{
	printf("Reg Line %d is acquired lock by \n",lineID);
	regLineLock->Acquire();
}
void cLine::regRelease()
{
	printf("Reg Line %d is release lock by \n",lineID);
	regLineLock->Release();
}
void cLine::Acquire(char *name,int id)
{

	printf("Both Line %d is acquired lock by %s %d \n",lineID,name,id);
	preferAcquire();
	regAcquire();
}
void cLine::Release(char *name,int id)
{
	printf("Both Line %d is release lock by \n",lineID,name,id);
	preferRelease();
	regRelease();
}
bool cLine::IsRegLineEmpty()
{
	bool count =  (regLineCount == 0);
	return count;
}
bool cLine::IsPreferLineEmpty()
{
	bool count =  (preferLineCount == 0);
	return count;
}

void cLine::addPreferLine(int c,int mount)
{
	printf("Customer X  in the prefer line %d\n",lineID);
	preferLineCount++;
	amount+=mount;//receive money from customer
	preferLineQueue->Append((void *)c);
	preferLineCond->Wait(preferLineLock);
	printf("Customer X in front the prefer line %d\n",lineID);
	
}
void cLine::addRegLine()	
{

}
void * cLine::getNextPreferLineCustomer(int clerkID)
{
	if(IsPreferLineEmpty()){
		printf("No customer in prefer line %d\n",lineID);
		preferLineLock->Release();
		return NULL;
	}
	preferLineCount--;
	preferLineCond->Signal(preferLineLock);
	printf("Clerk %d call next customer in prefer line %d\n",clerkID,lineID);
	return preferLineQueue->Remove();
}
void * cLine::getNextRegLineCustomer(int clerkID)
{
return NULL;

}
