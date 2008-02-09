#include "table.h"

Table::Table(int ID_in)
{
	tableID = ID_in;
	tableMoney = 0;
	cCount = 0;
	tableLock = new Lock("Table Lock");
	tableCondition = new Condition("Table Condiiton");
}


int Table::clerkCount()
{
	return cCount;
}

void Table::addClerk()
{
	cCount++;
	tableCondition.signal(&tableLock);
}

void Table::leaveTable()
{
	cCount--;
	tableCondiiton.wait(&tableLock);
}

void Table::acquireLock()
{
	tableLock.Acquire();
}

void Table::releaseLock()
{
	tableLock.Release();
}
	
void Table::addMoney(int amount)
{
	tableMoney += amount;
}

