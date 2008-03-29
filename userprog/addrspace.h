// addrspace.h 
//	Data structures to keep track of executing user programs 
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "table.h"

#define UserStackSize		1024 	// increase this as necessary!

#define MaxOpenFiles 256
#define MaxChildSpaces 256

#ifndef PROJ2
#define PROJ2
#endif
#ifndef PROJ3
#define PROJ3
#endif

#ifdef PROJ3
enum PageLocation{MAIN,SWAP,EXEC};
enum PageType{CODE,DATA,MIXED};
class VmTranslationEntry {
/*
   1. Physical Page
   2. Virtual Page
   3. Page Type (code, data, mixed, etc.)
   4. Page Location (main memory, swapfile, executable, etc.)
   5. Dirty Bit (whether a page has been modified -- VERY important)
   6. Use Bit (Never did find anywhere that this is used...)
   7. Process ID (Important because there are multiple virtual page Xs)
   8. Timestamp (For Nachos LRU page replacement later)
   9. Swap Address
 */
  public:
    int physicalPage;  	// The page number in real memory (relative to the
    int virtualPage;  	// The page number in virtual memory.
	PageType type;
	PageLocation location;
    bool dirty;         // This bit is set by the hardware every time the
    bool use;           // This bit is set by the hardware every time the
	int processID;
	int timeStamp;
    bool valid;         // If this bit is set, the translation is ignored.
    bool readOnly;	// If this bit is set, the user program is not allowed
	int swapAddr;
};


#endif
class AddrSpace {
  public:
    AddrSpace(OpenFile *executable);	// Create an address space,
					// initializing it with the program
					// stored in the file "executable"
    ~AddrSpace();			// De-allocate an address space

    void InitRegisters();		// Initialize user-level CPU registers,
					// before jumping to user code

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch
    Table fileTable;			// Table of openfiles
    unsigned int getNumPages(){return numPages;}
#ifdef PROJ2
	int getSpaceID();
	int getMaxForkAddr();
	int newStack();
#endif
#ifdef PROJ3
	int getCodeSize(){return itsCodeSize;}
	int getPageTableSize(){return itsPageTableSize;}
	int getProccessID(){return itsProccessID;}
	int getStackStartPage(){return itsStackStartPage;}
   VmTranslationEntry* getPageTable(){return pageTable;}
   int getFreeUserStack();
   void removeUserStack(int stackID);
#endif
 private:

#ifdef PROJ3
	
    VmTranslationEntry *pageTable;	// New entry table support vm 
#else
    TranslationEntry *pageTable;	// Assume linear page table translation
					// for now!
#endif
    unsigned int numPages;		// Number of pages in the virtual 
					// address space
#ifdef PROJ2
	int itsSpaceID;
	int itsMaxForkAddr;					
	bool valid;
	Lock *spaceLock;
#endif
#ifdef PROJ3
	int itsCodeSize;
	int itsPageTableSize;
	int itsProccessID;
	int itsStackStartPage;
	BitMap* itsUserStack;
	Lock* itsUserStackLock;
#endif
};

#endif // ADDRSPACE_H
