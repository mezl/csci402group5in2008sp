#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED
#include "list.h"

class Line
{
	public:
		Line();
		~Line();
		void addPreferLine();
		void addRegLine();
		void Line::preferAcquire();
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
