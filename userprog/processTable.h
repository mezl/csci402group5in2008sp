#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "bitmap.h"



class AddrSpace;
class Thread;

class ProcessTable
{
	public:
		ProcessTable();
		~ProcessTable();
		int InsertThread(Thread*);
		int RemoveThread(Thread*);

	private:
		//    std::map <int, std::vector<Thread*>> hashmap;
		Lock* processTableLock;
};



#endif