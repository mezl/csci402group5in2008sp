#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED
#include "list.h"

class Line
{
	public:
		Line();
		~Line();
		addPreferLine();
		addRegLine();
	private:
		int preferLineCount;		
		int regLineCount;		
		int lineID;
		int amount;
		Condition *preferLineCond;
		Lock *preferLineLock; 
		Condition *regLineCond;
		Lock *regLineLock;
	        List *preferLineQueue;	
	        List *regLineQueue;	
};
#endif
