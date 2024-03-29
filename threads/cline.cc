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
	//lineLock= new Lock("Customer both cLine Lock") ;	

	preferLineCount = 0;
	regLineCount = 0;
	lineID = ID;
	lineName = name;
	amount = 0;
	callNext = 0;
	regCallNext = 0;
}
cLine::~cLine()
{

}
void cLine::preferAcquire(char *name,int id,bool display)
{	
	if(display)printf("[Line]Prefer %s%d is acquired lock by %s %d\n",lineName,lineID,name,id);
	preferLineLock->Acquire();
	if(display)printf("[Line]Get prefer lock\n");
}
void cLine::preferRelease(char *name,int id,bool display)
{
	display = true;
	if(display)printf("[Line]Prefer %s%d is release lock by %s %d\n",lineName,lineID,name,id);
	preferLineLock->Release();
}

void cLine::regAcquire(char *name,int id,bool display)
{
	if(display)printf("[Line]Reg %s%d is acquired lock by %s %d\n",lineName,lineID,name,id);
	regLineLock->Acquire();
}
void cLine::regRelease(char *name,int id,bool display)
{
	display = true;
	if(display)printf("[Line]Reg %s%d is release lock by %s %d\n",lineName,lineID,name,id);
	regLineLock->Release();
}
void cLine::Acquire(char *name,int id,bool display)
{
	//lineLock->Acquire();
	if(display)printf("[Line]Both %s%d is acquired lock by %s %d \n",lineName,lineID,name,id);
	preferAcquire(name,id,false);
	regAcquire(name,id);
	//lineLock->Release();
}
void cLine::Release(char *name,int id,bool display)
{
	//lineLock->Acquire();
	if(display)printf("[Line]Both %s%d is release lock by %s %d\n",lineName,lineID,name,id);
	preferRelease(name,id,false);
	regRelease(name,id);
	//lineLock->Release();
}
bool cLine::IsRegLineEmpty()
{
	//lineLock->Acquire();
	bool count =  (regLineCount == 0);
	//lineLock->Release();
	return count;
}
bool cLine::IsPreferLineEmpty()
{
	//lineLock->Acquire();
	bool count =  (preferLineCount == 0);
	//lineLock->Release();
	return count;
}

void cLine::addPreferLine(int c,int mount)
{
	printf("[Line]One Customer go the prefer %s%d\n",lineName,lineID);
	//lineLock->Acquire();
	preferLineCount++;
	//lineLock->Release();
	printf("[Line]Prefer %s%d have[%d]Customer in the line\n",lineName,lineID,preferLineCount);
	amount+=mount;//receive money from customer
	preferLineQueue->Append((void *)c);	
	//while(callNext == 0)
		preferLineCond->Wait(preferLineLock);
	callNext --;
	printf("[Line]One Customer in front the prefer %s%d\n",lineName,lineID);
	
}
void cLine::addRegLine(int c)	
{
	printf("[Line]One Customer go the reg %s%d\n",lineName,lineID);
	//lineLock->Acquire();
	regLineCount++;
	//lineLock->Release();
	printf("[Line]Reg %s%d have[%d]Customer in the line\n",lineName,lineID,regLineCount);
	regLineQueue->Append((void *)c);

	//while(regCallNext == 0)
		regLineCond->Wait(regLineLock);
	regCallNext --;

	printf("[Line]One Customer in front the reg %s%d\n",lineName,lineID);
}
void * cLine::getNextPreferLineCustomer(char *clerkName,int clerkID)
{
		if(IsPreferLineEmpty()){
				printf("[Line]No customer in prefer %s%d\n",lineName,lineID);
				//preferLineLock->Release();
		}else{
				//	lineLock->Acquire();
				preferLineCount--;
				//	lineLock->Release();
				printf("[Line]Prefer %s%d have[%d]Customer in the line\n",lineName,lineID,preferLineCount);
				callNext++;
				preferLineCond->Signal(preferLineLock);
				printf("[Line]%s %d call next customer in prefer %s%d\n",clerkName,clerkID,lineName,lineID);
				return preferLineQueue->Remove();
		}
		return NULL;
}
void * cLine::getNextRegLineCustomer(char *clerkName,int clerkID)
{
	if(IsRegLineEmpty()){
		printf("[Line]No customer in reg %s%d\n",lineName,lineID);
		//regLineLock->Release();
		//return NULL;
	}else{
			//	lineLock->Acquire();
			regLineCount--;
			//	lineLock->Release();
			regCallNext++;
			printf("[Line]Reg %s%d have[%d]Customer in the line\n",lineName,lineID,regLineCount);
			regLineCond->Signal(regLineLock);
			printf("[Line]%s %d call next customer in reg %s%d\n",clerkName,clerkID,lineName,lineID);
			return regLineQueue->Remove();
	}
	return NULL;
}

int cLine::reportMoney()
{
	int m = 0;
	preferLineLock->Acquire();
	m = amount;
	preferLineLock->Release();
	return m;
}

bool cLine::nobody()
{
	bool state = false;

	if(IsRegLineEmpty() && IsPreferLineEmpty())
		return true;
	return false;
}
bool cLine::preferNeedClerk()
{
	bool state = false;
	preferLineLock->Acquire();
	if(preferLineCount>=1 || preferLineCount >=3)
		state = true;
	preferLineLock->Release();
	return state;
}
bool cLine::regNeedClerk()
{

	bool state = false;
	regLineLock->Acquire();
	if(regLineCount>=1 || regLineCount >=3)
		state = true;
	regLineLock->Release();
	return state;

}
