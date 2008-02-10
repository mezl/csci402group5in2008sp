// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"
#include <iostream>

using namespace std;

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P() // wait
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V() // signal
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!

#ifdef CHANGED

Lock::Lock(char* debugName) { 
  name = debugName; // for debugging purposes
  queue = new List; // will hold the FIFO queue of thread waiting on the lock
  owner = NULL;     // initially no thread owns the lock
}

Lock::~Lock() { delete queue; owner = NULL; }

//-----------------------
// Acquire() - if the Lock is ownerless, grants Lock to calling thread,
//             otherwise puts calling thread into a wait queue waiting on
//             the Lock to be Released
//-----------------------

void Lock::Acquire() {
  IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts
  
  // a thread can only acquire a Lock if no other thread has acquired it
  if(owner == NULL){ // grant the Lock to the currentThread
    owner = currentThread;
  }
  // if the currentThread already owns the Lock then we do nothing
  else if(currentThread != owner){ // queue the current thread because the Lock is busy
      queue->Append((void *)currentThread);
      currentThread->Sleep(); // thread will sleep until explicitly placed on ready queue
  }

  (void) interrupt->SetLevel(oldLevel); // enable interrupts
}

//----------------------
// Release() - grants ownership of the Lock to the next thread
//             in the wait queue if there is one, makes the Lock
//             ownerless if no threads are waiting to Acquire it
//----------------------

void Lock::Release() {
  Thread *thread;

  IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

  if(isHeldByCurrentThread()){ // A Lock may only be released by the thread that owns it

    // Now wake the next thread in line if there is one
    thread = (Thread *)queue->Remove();
    if (thread != NULL){	   // make thread ready
	scheduler->ReadyToRun(thread);
	owner = thread; // give the lock to the just woken up thread
    }else{//if no waiting threads, clear the lock
        owner = NULL; // remove the owner of the lock
    }

  }

  (void) interrupt->SetLevel(oldLevel); // enable interrupts
}

//---------------------
// isHeldByCurrentThread() - will return true if the Lock is owned by
//                           the thread in the currentThread variable,
//                           false otherwise
//---------------------

bool Lock::isHeldByCurrentThread(){
  return (currentThread == owner) ? true : false;
}

Condition::Condition(char* debugName) { name = debugName; lock = NULL; queue = new List; }
Condition::~Condition() { lock = NULL; delete queue; }

//---------------------
// Wait() - this will put the calling thread to sleep until the
//          Condition is Signalled or Broadcasted and associates
//          the Condition with the Lock passed in as conditionLock
//---------------------
void Condition::Wait(Lock* conditionLock) {
  IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

  // only the thread owning the Condition's Lock can Wait
  if(conditionLock->isHeldByCurrentThread()){
    if(lock == NULL){
      lock = conditionLock;
    }
    conditionLock->Release();
    queue->Append((void *)currentThread); // add currentThread to waiting queue
    currentThread->Sleep(); // thread will sleep until explicitly put back on ready queue
    conditionLock->Acquire();
  }

  (void) interrupt->SetLevel(oldLevel); // enable interrupts
}

//---------------------
// Signal() - this will wake the first thread in the wait queue waiting on the Condition
//            note: Locks and Conditions have separate wait queues
//---------------------
void Condition::Signal(Lock* conditionLock) {
  IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts
  Thread *thread;

  // only the thread owning the Condition's Lock can Signal
  if(conditionLock != lock){
     printf("%s is trying to Signal on Condition %s with the wrong lock\n",currentThread->getName(), name);
  }else if(conditionLock->isHeldByCurrentThread()){
    // Now wake the next thread in line if there is one
    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready
      scheduler->ReadyToRun(thread);
  }

  (void) interrupt->SetLevel(oldLevel); // enable interrupts
}

//---------------------
// Broadcast() - this will wake all threads waiting in the wait queue waiting on the Condition
//               note: Locks and Conditions have separate wait queues
//---------------------
void Condition::Broadcast(Lock* conditionLock) {

  while(!queue->IsEmpty())
    Signal(conditionLock);

}

#else
Lock::Lock(char* debugName) {}
Lock::~Lock() {}
void Lock::Acquire() {}
void Lock::Release() {}
Condition::Condition(char* debugName) { }
Condition::~Condition() { }
void Condition::Wait(Lock* conditionLock) { ASSERT(FALSE); }
void Condition::Signal(Lock* conditionLock) { }
void Condition::Broadcast(Lock* conditionLock) { }
#endif
