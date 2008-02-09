// table.h

#ifdef  TABLE_H_DEFINED
#define TABLE_H_DEFINED
#include "synch.h"


class Table
{
	public:
		Table(int ID_in);
		~Table();
		int clerkCount();
		void addClerk();
		void leaveTable();
		void acquireLock();
		void releaseLock();
		void addMoney(int amount);


	private:
		int tableID;
		int tableMoney;
		int clerkCount;
		Lock *tableLock;
		Condition *tableCondition;
};
#endif
