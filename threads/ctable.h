#ifndef TABLE_H_DEFINED
#define TABLE_H_DEFINED
#include "list.h"
#include "synch.h"
#include "system.h"


class cTable
{
	public:
		cTable(int ID_in);
		~cTable(){}
		int clerkCount();
		void addClerk(char *name,int id);
		void leaveTable(char *name,int id);
		void acquireLock(char *name,int id);
		void releaseLock(char *name,int id);
		void addMoney(int amount);
	private:
		int tableID;
		int tableMoney;
		int cCount;
		Lock *tableLock;
		Condition *tableCondition;
};
#endif
