#include "line.h"

Line::Line(int ID)
{
	preferLineCond = new Condition("Customer Prefer Line Cond") ;	
	preferLineLock= new Lock("Customer Prefer Line Lock") ;	
	preferLineQueue = new List;
	regLineCond = new Condition("Customer Prefer Line Cond") ;	
	regLineLock= new Lock("Customer Prefer Line Lock") ;	
	regLineQueue = new List;

	preferLineCount = 0;
	regLineCount = 0;
	lineID = ID;
	amount = 0;
}
Line::~Line()
{

}
void Line::preferAcquire()
{
	preferLineLock->Acquire();
}
void Line::preferRelease()
{
	preferLineLock->Release();
}

void Line::regAcquire()
{
	regLineLock->Acquire();
}
void Line::regRelease()
{
	regLineLock->Release();
}
void Line::Acquire()
{
	preferAcquire();
	regAcquire();
}
void Line::Release()
{
	preferRelease();
	regRelease();
}
bool Line::IsRegLineEmpty()
{
	bool count =  (regLineCount == 0);
	return count;
}
bool Line::IsPreferLineEmpty()
{
	bool count =  (preferLineCount == 0);
	return count;
}
void Line::addPreferLine(Customer c,int mount)
{
	printf("Customer %d in the prefer line %d\n",c.getID(),lineID);
	preferLineCount++;
	amount+=mount;//receive money from customer
	preferLineQueue->Append((void *)c);
	preferLineCond->Wait(&preferLineLock);
	printf("Customer %d in front the prefer line %d\n",c.getID(),lineID);
	
}
void Line::addRegLine()	
{

}
Customer * Line::getNextPreferLineCustomer(int clerkID)
{
	if(IsPreferLineEmpty()){
		printf("No customer in prefer line %d\n",lineID);
		preferLineLock->Release();
		return;
	}
	preferLineCount--;
	preferLineCond->Singal(&preferLineLock);
	printf("Clerk %d call next customer in prefer line %d\n",clerkID,lineID);
	return (Customer *)preferLineQueue->Remove();
}

