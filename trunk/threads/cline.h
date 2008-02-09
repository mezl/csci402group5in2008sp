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
		void preferAcquire(char *name,int id);
		void preferRelease(char *name,int id);
		void regAcquire(char *name,int id);
		void regRelease(char *name,int id);
		void Acquire(char *name,int id);
		void Release(char *name,int id);
		bool IsRegLineEmpty();
		bool IsPreferLineEmpty();
		int getID(){return lineID;}
		char *getName(){return lineName;}
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
	        List* preferLineQueue;	
	        List* regLineQueue;	
};
#endif
