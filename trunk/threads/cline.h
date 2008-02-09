#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED
#include "list.h"
#include "synch.h"
#include "system.h"
//#include "customer.h"
class cLine
{
	public:
		cLine(int ID);
		cLine();
		~cLine();
		void addPreferLine(void * c,int mount);
		void addRegLine();
		void * getNextPreferLineCustomer(int clerkID);
		void preferAcquire();
		void preferRelease();
		void regAcquire();
		void regRelease();
		void Acquire();
		void Release();
		bool IsRegLineEmpty();
		bool IsPreferLineEmpty();
	private:
		int preferLineCount;		
		int regLineCount;		
		int lineID;
		int amount;
		Condition* preferLineCond;
		Lock* preferLineLock; 
		Condition* regLineCond;
		Lock* regLineLock;
	        List* preferLineQueue;	
	        List* regLineQueue;	
};
#endif
