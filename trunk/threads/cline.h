#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED
#include "list.h"
#include "synch.h"
#include "customer.h"
#include "system.h"
class Line
{
	public:
		cLine(int ID);
		~cLine();
		void addPreferLine(Customer c,int mount);
		void addRegLine();
		Customer * getNextPreferLineCustomer(int clerkID);
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