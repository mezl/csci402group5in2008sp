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
//if there is no vm,then we make physmemory large
#ifndef PROJ3
#endif
extern Machine* machine;	// user program memory and registers

//-------------------- Project 2 (part 1) -------------------
extern Table lockTable;
extern Table conditionTable;

//-------------------- Project 2 (part 2) -------------------
extern ProcessTable processTable;
extern ProcessTable threadTable;
extern Table memoryTable;

#endif

//-------------------- Project 3 (part 1 & 2) -------------------
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
//-------------------- Project 4 (part 1) -------------------
#ifdef PROJ4
#define SERVER_NUM 3
extern Lock* mailBoxLock;
extern Lock* clerkRegLock;
extern Lock* customerAcquireLock;
extern BitMap* mailBoxMap;
extern int machineID;
enum CLERK_TYPE{APP,PIC,PASS,CASH};

class Clerk_Table{
   public:
   int type;//four clerk type
   List* clerkTable;
   Clerk_Table(int t){
      type = t;
      clerkTable= new List();
   }
   ~Clerk_Table(){
      delete clerkTable;
   }
   
};
extern Clerk_Table* AppClerkTable;
extern Clerk_Table* PicClerkTable;
extern Clerk_Table* PassClerkTable;
extern Clerk_Table* CashClerkTable;
extern List* AppLine;
extern List* PicLine;
extern List* PassLine;
extern List* CashLine;
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
