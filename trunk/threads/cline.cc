#include "cline.h"
cLine::cLine()
{

}
cLine::cLine(char *name,int ID)
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
	lineName = name;
	amount = 0;
}
cLine::~cLine()
{

}
void cLine::preferAcquire(char *name,int id)
{
	printf("Prefer Line %d is acquired lock by %s %d\n",lineID,name,id);
	preferLineLock->Acquire();
}
void cLine::preferRelease(char *name,int id)
{
	printf("Prefer Line %d is release lock by %s %d\n",lineID,name,id);
	preferLineLock->Release();
}

void cLine::regAcquire(char *name,int id)
{
	printf("Reg Line %d is acquired lock by %s %d\n",lineID,name,id);
	regLineLock->Acquire();
}
void cLine::regRelease(char *name,int id)
{
	printf("Reg Line %d is release lock by %s %d\n",lineID,name,id);
	regLineLock->Release();
}
void cLine::Acquire(char *name,int id)
{

	printf("Both Line %d is acquired lock by %s %d \n",lineID,name,id);
	preferAcquire(name,id);
	regAcquire(name,id);
}
void cLine::Release(char *name,int id)
{
	printf("Both Line %d is release lock by %s %d\n",lineID,name,id);
	preferRelease(name,id);
	regRelease(name,id);
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
	printf("[Line]One Customer go the prefer %s %d\n",lineName,lineID);
	preferLineCount++;
	printf("[Line]Prefer %s %d have %d Customer in the line\n",lineName,lineID,preferLineCount);
	amount+=mount;//receive money from customer
	preferLineQueue->Append((void *)c);
	preferLineCond->Wait(preferLineLock);
	printf("[Line]Customer X in front the prefer  %s %d\n",lineName,lineID);
	
}
void cLine::addRegLine(int c)	
{

}
void * cLine::getNextPreferLineCustomer(int clerkID)
{
	if(IsPreferLineEmpty()){
		printf("[Line]No customer in prefer line %d\n",lineID);
		preferLineLock->Release();
		return NULL;
	}
	preferLineCount--;
	preferLineCond->Signal(preferLineLock);
	printf("[Line]Clerk %d call next customer in prefer line %d\n",clerkID,lineID);
	return preferLineQueue->Remove();
}
void * cLine::getNextRegLineCustomer(int clerkID)
{
return NULL;

}
