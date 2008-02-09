#include "ctable.h"

cTable::cTable(int ID_in)
{
	tableID = ID_in;
	tableMoney = 0;
	cCount = 0;
	tableLock = new Lock("Table Lock");
	tableCondition = new Condition("Table Condiiton");
}


int cTable::clerkCount()
{
	return cCount;
}

void cTable::addClerk(char *name,int id)
{
	cCount++;
	tableCondition->Signal(tableLock);
}

void cTable::leaveTable(char *name,int id)
{
	cCount--;
	printf("[Table]%s %d is leave table now,[%d]still work\n",name,id,cCount);
	tableCondition->Wait(tableLock);
	printf("[Table]%s %d is come back now\n",name,id);
}

void cTable::acquireLock(char *name,int id)
{
	tableLock->Acquire();
}

void cTable::releaseLock(char *name,int id)
{
	tableLock->Release();
}
	
void cTable::addMoney(int amount)
{
	tableMoney += amount;
}

