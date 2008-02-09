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
	preferLineLock->Acquire();
}
void cLine::preferRelease()
{
	preferLineLock->Release();
}

void cLine::regAcquire()
{
	regLineLock->Acquire();
}
void cLine::regRelease()
{
	regLineLock->Release();
}
void cLine::Acquire()
{
	preferAcquire();
	regAcquire();
}
void cLine::Release()
{
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
//	printf("Customer %d in the prefer line %d\n",c.getID(),lineID);
	preferLineCount++;
	amount+=mount;//receive money from customer
	preferLineQueue->Append((void *)c);
	preferLineCond->Wait(preferLineLock);
//	printf("Customer %d in front the prefer line %d\n",c.getID(),lineID);
	
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
	preferLineCond->Singal(preferLineLock);
	printf("Clerk %d call next customer in prefer line %d\n",clerkID,lineID);
	return preferLineQueue->Remove();
}
