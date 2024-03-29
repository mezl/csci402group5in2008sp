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

//using namespace std; 

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
Semaphore::P()
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
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
	//Add by Kai
	name = debugName;
	lockWaitQueue = new List;
	lockOwner = NULL;
}
Lock::~Lock() {
	//Add by Kai 
	delete lockWaitQueue;
	//lockOwner = NULL;
}
void Lock::Acquire() {
	//Add by Kai	
	//Disable interrupts
	IntStatus oldLevel = interrupt->SetLevel(IntOff);

	//If lock is avaiable
	if(lockOwner == NULL){
		//I get Lock
		//Make my self lock owner
		lockOwner = currentThread;
		
	}else if(currentThread != lockOwner){//Lock is not avaiable

		//Add myself to lock wait queue
		lockWaitQueue->Append((void *)currentThread);	// so go to sleep
		//Go to sleep
		currentThread->Sleep();
	
	}
	//Restore Interrupt
	(void) interrupt->SetLevel(oldLevel);
	//Above Done by Kai

}
void Lock::Release() {
	//Disable interrupts
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
   
	//Add by Kai
	Thread *thread;


	//Check lock ownership
	if(isHeldByCurrentThread())
	{
		//Check for a waiting thread in lock waiting queue
		thread = (Thread *)lockWaitQueue->Remove();
		if (thread != NULL){	   //If found thread in lock wait queue
	       //1.Wake Next thread up - put it on ReadyToRun Queue
		scheduler->ReadyToRun(thread);
	       //2.Remove thread from queue(we did it before)
	       //3.Make the thread the lock owner
		lockOwner = thread;
		}else{//No thread in lock waiting queue
			lockOwner = NULL;//clear lock ownership	
		}
	}else{// if held by current thread
	printf("[ThreadMsg]%s Release is not handle by current thread\n",currentThread->getName());
	//printf("[ThreadMsg]Lock owner is %s \n",lockOwner->getName());
	}
	//Restore Interrupt
	(void) interrupt->SetLevel(oldLevel);
	//Above Done by Kai	
}
bool Lock::isHeldByCurrentThread(){
	if(currentThread == lockOwner)
		return true;
	return false;	
//	return (currentThread == lockOwner);
}
Condition::Condition(char* debugName) {

	//Add by Kai
	name = debugName;
	condWaitQueue = new List;
	condLock = NULL;
}
Condition::~Condition() {
	//Add by Kai 
	condLock = NULL;
	delete condWaitQueue;

}
void Condition::Wait(Lock* conditionLock) { 
	
	//Add by Kai	
	//Disable interrupts
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	//check saved lock is match the current lock
	if(conditionLock->isHeldByCurrentThread()){
		if(condLock == NULL)
			condLock = conditionLock;//Save lock pt first
		conditionLock->Release();//Leave Monitor
		//Add myself to condition wait queue
		condWaitQueue->Append((void *)currentThread);
		currentThread->Sleep(); // so go to sleep

		//After some one wake me up
		conditionLock->Acquire();//Reenter monitor
	}
	//Restore Interrupt
	(void) interrupt->SetLevel(oldLevel);
	//Don't know what's this
	//ASSERT(FALSE); 

}
void Condition::Signal(Lock* conditionLock) {

	//Add by Kai	
	//Disable interrupts
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	Thread *thread;
	if(conditionLock != condLock){
		//check saved lock is match the current lock
		printf("Thread %s try signal cond %s with wrong lock \n",currentThread->getName(),name);
	}else if(conditionLock->isHeldByCurrentThread()){
		//Check for a waiting thread in condition waiting queue
		thread = (Thread *)condWaitQueue->Remove();
		if (thread != NULL){
			//Wake Next thread up - put it on ReadyToRun Queue
			scheduler->ReadyToRun(thread);
			
		}
		if(condWaitQueue->IsEmpty()){
			//conditionLock->clearLockOwner();
		}

	}else{	
		//printf("Error, thread should not empty\n");
	}		
	
	//Restore Interrupt
	(void) interrupt->SetLevel(oldLevel);
}
void Condition::Broadcast(Lock* conditionLock) {
	//Add by Kai	
	while(!condWaitQueue->IsEmpty() ) {
		Signal(conditionLock);
	}

}
