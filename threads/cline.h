#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED
#include "list.h"
#include "synch.h"
#include "system.h"
//#include "customer.h"
class cLine
{
	public:
		cLine(char *name,int ID);
		cLine();
		~cLine();
		void addPreferLine(int c,int mount);
		void addRegLine(int c);
		void * getNextPreferLineCustomer(char *clerkName,int clerkID);
		void * getNextRegLineCustomer(char *clerkName,int clerkID);
		int preferCustomerCount(){return preferLineCount;}
		int regCustomerCount(){return regLineCount;}
		void preferAcquire(char *name,int id,bool display=true);
		void preferRelease(char *name,int id,bool display=true);
		void regAcquire(char *name,int id,bool display=true);
		void regRelease(char *name,int id,bool display=true);
		void Acquire(char *name,int id,bool display=true);
		void Release(char *name,int id,bool display=true);
		bool IsRegLineEmpty();
		bool IsPreferLineEmpty();
		int getID(){return lineID;}
		char *getName(){return lineName;}
		int reportMoney();


	private:
		int preferLineCount;		
		int regLineCount;		
		int lineID;
		char *lineName;
		int amount;
		Condition* preferLineCond;
		Lock* preferLineLock; 
		Condition* regLineCond;
		Lock* regLineLock;
		Lock* lineLock;//for lock both line
		List* preferLineQueue;	
		List* regLineQueue;	
};
#endif
