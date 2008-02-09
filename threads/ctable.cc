#include "table.h"

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

void cTable::addClerk()
{
	cCount++;
	tableCondition->Signal(tableLock);
}

void cTable::leaveTable()
{
	cCount--;
	tableCondition->Wait(tableLock);
}

void cTable::acquireLock()
{
	tableLock->Acquire();
}

void cTable::releaseLock()
{
	tableLock->Release();
}
	
void cTable::addMoney(int amount)
{
	tableMoney += amount;
}

