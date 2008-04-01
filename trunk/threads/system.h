// system.h 
//	All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"

#ifndef PROJ3
#define PROJ3
#endif
// Initialization and cleanup routines
extern void Initialize(int argc, char **argv); 	// Initialization,
						// called before anything else
extern void Cleanup();				// Cleanup, called when
						// Nachos is done.

extern Thread *currentThread;			// the thread holding the CPU
extern Thread *threadToBeDestroyed;  		// the thread that just finished
extern Scheduler *scheduler;			// the ready list
extern Interrupt *interrupt;			// interrupt status
extern Statistics *stats;			// performance metrics
extern Timer *timer;				// the hardware alarm clock

#ifdef USER_PROGRAM
#include "bitmap.h"
#include "machine.h"
#include "synch.h"

extern Machine* machine;	// user program memory and registers

//-------------------- Project 2 (part 1) -------------------
extern Table lockTable;
extern Table conditionTable;

//-------------------- Project 2 (part 2) -------------------
extern ProcessTable processTable;
extern ProcessTable threadTable;
extern Table memoryTable;

#endif

#ifdef USER_PROGRAM
#ifdef PROJ3
class IPTEntry {
	public:
	int pid;//Proccess ID
	int vpn;//Virtual page number
	bool use;
	bool dirty;
	bool valid;
	bool readOnly;
	AddrSpace* space;
};
extern IPTEntry* IPTable;
enum IPT_Replace_Algorithm{FIFO,RAND};
extern IPT_Replace_Algorithm ipt_replace_algorithm; 
extern BitMap* swapFileMap;
extern OpenFile* swapfile;
extern int nextEvictIPTSlot;
extern int nextEvictTLBSlot;
extern Lock* physMemoryLock;
extern int processID_Counter;
#endif 
#endif 
#ifdef FILESYS_NEEDED 		// FILESYS or FILESYS_STUB 
#include "filesys.h"
extern FileSystem  *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk   *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice* postOffice;
#endif

#endif // SYSTEM_H
