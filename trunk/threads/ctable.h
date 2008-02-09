#ifndef TABLE_H_DEFINED
#define TABLE_H_DEFINED
#include "list.h"
#include "synch.h"
#include "system.h"


class cTable
{
	public:
		cTable(int ID_in, int initialClerkCount);
		~cTable(){}
		int clerkCount();
		void addClerk(char *name,int id,bool display = false);
		void leaveTable(char *name,int id,bool display = false);
		void acquireLock(char *name,int id,bool display = false);
		void releaseLock(char *name,int id,bool display = false);
		void addMoney(int amount);
		int reportMoney();

	private:
		int tableID;
		int tableMoney;
		int cCount;
		Lock *tableLock;
		Condition *tableCondition;
};
#endif
