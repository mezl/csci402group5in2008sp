#include "table.h"
Table::Table()
{

}

Table::Table(int ID_in)
{
	tableID = ID_in;
	tableMoney = 0;
	clerkCount = 0;
	tableLock = new Lock("Table Lock");
	tableCondiiton = new Condiiton("Table Condiiton");
}

Table::~Table()
{
}

int Table::clerkCount()
{
	return clerkCount;
}

void Table::addClerk()
{
	clerkCount++;
	tableCondition.signal(&tableLock);
}

void Table::leaveTable()
{
	clerkCount--;
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

