#include "ctable.h"

cTable::cTable(int ID_in, int initialClerkCount)
{
	tableID = ID_in;
	tableMoney = 0;
	cCount = initialClerkCount;
	leaveCount = 0;
	tableLock = new Lock("Table Lock");
	tableCondition = new Condition("Table Condiiton");
	needClerk = 0;
}


int cTable::clerkCount()
{
	return cCount-leaveCount;
}

void cTable::addClerk(char *name,int id,bool display)
{
	display = true;
	if(leaveCount == 0){
		if(display)printf("[Table]No clerk can add!\n");
		return;
	}
	//cCount++;
	leaveCount--;
	needClerk  ++;
	if(display)printf("[Table]%s%d table %d is prepare adding a clerk\n", name,id,tableID);
	tableCondition->Signal(tableLock);
	if(display)printf("[Table]%s%d table %d is added a clerk\n",name,id, tableID);
}

void cTable::leaveTable(char *name,int id,bool display)
{
	//cCount--;
	leaveCount++;
	if(display)printf("[Table]%s %d is leave table now,[%d]still work[%d]leave\n",name,id,cCount,leaveCount);
	while(needClerk == 0)
		tableCondition->Wait(tableLock);
	needClerk--;
	if(display)printf("[Table]%s %d is come back now\n",name,id);
}

void cTable::acquireLock(char *name,int id,bool display)
{
	if(display)printf("[Table]%s %d is acquire lock now\n",name,id);
	tableLock->Acquire();
	if(display)printf("[Table]%s %d is get lock now\n",name,id);
}

void cTable::releaseLock(char *name,int id,bool display)
{
	if(display)printf("[Table]%s %d is prepare releasing lock now\n",name,id);
	tableLock->Release();
	if(display)printf("[Table]%s %d is released lock now\n",name,id);
}
	
void cTable::addMoney(int amount)
{
	tableMoney += amount;
}

int cTable::reportMoney()
{
	return tableMoney;
}
