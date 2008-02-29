#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "bitmap.h"
#include <map>
#include <vector>


class AddrSpace;
class Thread;

class ProcessTable
{
	public:
		ProcessTable();
		~ProcessTable();
		int AddThread(Thread*);
		int RemoveThread(Thread*);
		int CheckChildExist(int mySpaceId);

	private:
		std::map <int, std::vector<Thread*> > hashmap;
		Lock* processTableLock;
};



#endif
