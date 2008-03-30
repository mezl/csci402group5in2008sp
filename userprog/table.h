// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef TABLE_H
#define TABLE_H

#include "bitmap.h"

class Lock;

class Table {
    BitMap map;
    void **table;
    Lock *lock;
    int size;
 public:
    Table(int);
    ~Table();
    void *Get(int);
    int Put(void *);
    void *Remove(int);
};


#endif // TABLE_H
#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

class Thread;
class AddrSpace;
#include <map>
#include <vector>

class ProcessTable
{
	public:
		ProcessTable();
		~ProcessTable();
		int AddThread(Thread*);
		int getProcessTable(Thread*);
		int getProcess(Thread*);
		int RemoveThread(Thread*);
		int CheckChildExist(int mySpaceId);

	private:
		std::map <int, std::vector<Thread*> > hashmap;
		Lock* processTableLock;
};



#endif
