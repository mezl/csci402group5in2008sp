// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "../threads/system.h"
#include "syscall.h"
#include <stdio.h>
#include <iostream>
#include "../threads/synch.h"

using namespace std;

int copyin(unsigned int vaddr, int len, char *buf) {
    // Copy len bytes from the current thread's virtual address vaddr.
    // Return the number of bytes so read, or -1 if an error occors.
    // Errors can generally mean a bad virtual address was passed in.
    bool result;
    int n=0;			// The number of bytes copied in
    int *paddr = new int;

    while ( n >= 0 && n < len) {
      result = machine->ReadMem( vaddr, 1, paddr );
      buf[n++] = *paddr;

      if ( !result ) {
	//translation failed
	return -1;
      }

      vaddr++;
    }

    delete paddr;
    return len;
}

int copyout(unsigned int vaddr, int len, char *buf) {
    // Copy len bytes to the current thread's virtual address vaddr.
    // Return the number of bytes so written, or -1 if an error
    // occors.  Errors can generally mean a bad virtual address was
    // passed in.
    bool result;
    int n=0;			// The number of bytes copied in

    while ( n >= 0 && n < len) {
      // Note that we check every byte's address
      result = machine->WriteMem( vaddr, 1, (int)(buf[n++]) );

      if ( !result ) {
	//translation failed
	return -1;
      }

      vaddr++;
    }

    return n;
}

void Create_Syscall(unsigned int vaddr, int len) {
    // Create the file with the name in the user buffer pointed to by
    // vaddr.  The file name is at most MAXFILENAME chars long.  No
    // way to return errors, though...
    char *buf = new char[len+1];	// Kernel buffer to put the name in

    if (!buf) return;

    if( copyin(vaddr,len,buf) == -1 ) {
	printf("%s","Bad pointer passed to Create\n");
	delete buf;
	return;
    }

    buf[len]='\0';

    fileSystem->Create(buf,0);
    delete[] buf;
    return;
}

int Open_Syscall(unsigned int vaddr, int len) {
    // Open the file with the name in the user buffer pointed to by
    // vaddr.  The file name is at most MAXFILENAME chars long.  If
    // the file is opened successfully, it is put in the address
    // space's file table and an id returned that can find the file
    // later.  If there are any errors, -1 is returned.
    char *buf = new char[len+1];	// Kernel buffer to put the name in
    OpenFile *f;			// The new open file
    int id;				// The openfile id

    if (!buf) {
	printf("%s","Can't allocate kernel buffer in Open\n");
	return -1;
    }

    if( copyin(vaddr,len,buf) == -1 ) {
	printf("%s","Bad pointer passed to Open\n");
	delete[] buf;
	return -1;
    }

    buf[len]='\0';

    f = fileSystem->Open(buf);
    delete[] buf;

    if ( f ) {
	if ((id = currentThread->space->fileTable.Put(f)) == -1 )
	    delete f;
	return id;
    }
    else
	return -1;
}

void Write_Syscall(unsigned int vaddr, int len, int id) {
    // Write the buffer to the given disk file.  If ConsoleOutput is
    // the fileID, data goes to the synchronized console instead.  If
    // a Write arrives for the synchronized Console, and no such
    // console exists, create one. For disk files, the file is looked
    // up in the current address space's open file table and used as
    // the target of the write.
    
    char *buf;		// Kernel buffer for output
    OpenFile *f;	// Open file for output

    if ( id == ConsoleInput) return;
    
    if ( !(buf = new char[len]) ) {
	printf("%s","Error allocating kernel buffer for write!\n");
	return;
    } else {
        if ( copyin(vaddr,len,buf) == -1 ) {
	    printf("%s","Bad pointer passed to to write: data not written\n");
	    delete[] buf;
	    return;
	}
    }

    if ( id == ConsoleOutput) {
      for (int ii=0; ii<len; ii++) {
	printf("%c",buf[ii]);
      }

    } else {
	if ( (f = (OpenFile *) currentThread->space->fileTable.Get(id)) ) {
	    f->Write(buf, len);
	} else {
	    printf("%s","Bad OpenFileId passed to Write\n");
	    len = -1;
	}
    }

    delete[] buf;
}

int Read_Syscall(unsigned int vaddr, int len, int id) {
    // Write the buffer to the given disk file.  If ConsoleOutput is
    // the fileID, data goes to the synchronized console instead.  If
    // a Write arrives for the synchronized Console, and no such
    // console exists, create one.    We reuse len as the number of bytes
    // read, which is an unnessecary savings of space.
    char *buf;		// Kernel buffer for input
    OpenFile *f;	// Open file for output

    if ( id == ConsoleOutput) return -1;
    
    if ( !(buf = new char[len]) ) {
	printf("%s","Error allocating kernel buffer in Read\n");
	return -1;
    }

    if ( id == ConsoleInput) {
      //Reading from the keyboard
      scanf("%s", buf);

      if ( copyout(vaddr, len, buf) == -1 ) {
	printf("%s","Bad pointer passed to Read: data not copied\n");
      }
    } else {
	if ( (f = (OpenFile *) currentThread->space->fileTable.Get(id)) ) {
	    len = f->Read(buf, len);
	    if ( len > 0 ) {
	        //Read something from the file. Put into user's address space
  	        if ( copyout(vaddr, len, buf) == -1 ) {
		    printf("%s","Bad pointer passed to Read: data not copied\n");
		}
	    }
	} else {
	    printf("%s","Bad OpenFileId passed to Read\n");
	    len = -1;
	}
    }

    delete[] buf;
    return len;
}

void Close_Syscall(int fd) {
    // Close the file associated with id fd.  No error reporting.
    OpenFile *f = (OpenFile *) currentThread->space->fileTable.Remove(fd);

    if ( f ) {
      delete f;
    } else {
      printf("%s","Tried to close an unopen file\n");
    }
}


// -------------------------------------------------------
// -------------- PROJECT 2 (part 1) ---------------------
// -------------------------------------------------------


#ifdef USER_PROGRAM
int CreateLock_Syscall()
{
	// create a new lock
	Lock* myLock = new Lock("New Lock");
	
	// get index of the new lock
	int id;
	id = lockTable.Put(myLock);

	// failure creating a lock
	if(id < 0)
	{
		delete myLock;
		printf("Failure creating a lock of index %d \n", id);
	}

	// printf("[exception] returning lock %d \n", id);
	// return lock index
	return id;
}

void DestroyLock_Syscall(int id)
{
	if(processTable.CheckChildExist(currentThread->space->getSpaceID()) == 1)
	{
		Lock* myLock = (Lock*)lockTable.Remove(id);
		if(myLock == NULL)
		{
			printf("Failure deleting a lock of index %d \n", id);
			return;
		}
		delete myLock;
	}
	else
		printf("Unable to destroy Lock because not last child thread \n");
	
}

void Acquire_Syscall(int id)
{
	Lock* myLock = (Lock*)lockTable.Get(id);

	if(myLock == NULL)
	{
		printf("Failure Acquiring a lock of index %d \n", id);
		return;
	}
	else{
		myLock -> Acquire();}
}

void Release_Syscall(int id)
{
	Lock* myLock = (Lock*)lockTable.Get(id);

	if(myLock == NULL)
	{
		printf("Failure Releasing a lock of index %d \n", id);
		return;
	}
	else{
		myLock -> Release();}
}

int CreateCondition_Syscall()
{
	Condition* myCondition = new Condition("New Condition");
	int id = conditionTable.Put(myCondition);

	if(id < 0)
	{
		printf("Failure creating a condition of index %d \n", id);
		delete myCondition;
	}
	return id;
}

void DestroyCondition_Syscall(int id)
{	
	if(processTable.CheckChildExist(currentThread->space->getSpaceID()) == 1)
	{
		Condition* myCondition = (Condition*)conditionTable.Remove(id);
		if(myCondition == NULL)
		{	
			printf("Failure destroying a condition of index %d \n", id);
			return;
		}	
		delete myCondition;
	}
	else
		printf("Unable to destroy Condition because not last child thread \n");
	
}

void Signal_Syscall(int lockID, int conditionID)
{
	Condition* myCondition = (Condition*)conditionTable.Get(conditionID);
	Lock* myLock = (Lock*)lockTable.Get(lockID);

	if(myCondition == NULL)
	{
		printf("Failure Signaling a condition (condition %d dont exist)", conditionID);
		return;
	}	
	if(myLock == NULL)
	{
		printf("Failure Signaling a condition (lock %d dont exist)", lockID);	
		return;
	}
	
	myCondition -> Signal(myLock);
}

void Wait_Syscall(int lockID, int conditionID)
{
	Condition* myCondition = (Condition*)conditionTable.Get(conditionID);
	Lock* myLock = (Lock*)lockTable.Get(lockID);

	if(myCondition == NULL)
	{
		printf("Failure waiting a condition (condition %d dont exist)", conditionID);
		return;
	}	
	if(myLock == NULL)
	{
		printf("Failure waiting a condition (lock %d dont exist)", lockID);	
		return;
	}
	
	myCondition -> Wait(myLock);
}

void Broadcast_Syscall(int lockID, int conditionID)
{
	Condition* myCondition = (Condition*)conditionTable.Get(conditionID);
	Lock* myLock = (Lock*)lockTable.Get(lockID);

	if(myCondition == NULL)
	{
		printf("Failure broadcasting a condition (condition %d dont exist)", conditionID);
		return;
	}	
	if(myLock == NULL)
	{
		printf("Failure broadcasting a condition (lock %d dont exist)", lockID);	
		return;
	}
	
	myCondition -> Broadcast(myLock);
}

#endif

// ----------------------------------------------------
// --------------- PROJECT 2 PART 2 -------------------
// ----------------------------------------------------

#ifdef USER_PROGRAM

void exec_thread()
{
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
}
SpaceId Exec_Syscall(char* name)
{
	OpenFile* myFile = fileSystem->Open(name);
	AddrSpace* mySpace = new AddrSpace(myFile);
	Thread* myThread = new Thread(name);
	myThread->space = mySpace;
	int mySpaceId = myThread->space->getSpaceID();
	int myThreadId = processTable.AddThread(myThread);
	
	machine -> WriteRegister(2, mySpaceId);
	myThread->Fork((VoidFunctionPtr)exec_thread, 0);
	return mySpaceId;
}

void kernel_thread(int twoValues)
{
	int* twoValues_temp = (int*) twoValues;
	int virtualaddress = twoValues_temp[0];
	int stackAddress = twoValues_temp[1];

	int myIncrementPC = virtualaddress + 4;

	machine -> WriteRegister(PCReg, virtualaddress);	
	machine -> WriteRegister(NextPCReg, myIncrementPC);
	currentThread -> space -> RestoreState();
	machine -> WriteRegister(StackReg, stackAddress);
	machine -> Run();
}

void Fork_Syscall(int virtualaddress)
{
	Thread* myThread = new Thread(currentThread->getName());
	int* twoValues = new int[2];
	twoValues[0] = virtualaddress;
   
   //myThread->spaceID = currentThread->spaceID;

	int stackAddress = currentThread->space->newStack();
	myThread->space = currentThread->space;
	int mySpaceId = processTable.AddThread(myThread);

	if (stackAddress < 0)
	{
		printf("Failure when allocating a new stack for fork");
		return;
	}
	twoValues[1] = stackAddress;
	
	myThread->Fork((VoidFunctionPtr)kernel_thread, (int)twoValues);
}


void Yield_Syscall()
{	
	currentThread -> Yield();
}

void Exit_Syscall(int status)
{
	/*
	if(processTable.CheckChildExist(currentThread->space->getSpaceID()) == 1)
	{
		currentThread -> Sleep();
	}*/
	int check = processTable.RemoveThread(currentThread);
	
	if(check == -1)
	{
		printf("Can't exit current thread\n");
	}
	if(check == 1 || check == 2)
		currentThread->space->~AddrSpace();

	if(check == 2)
		interrupt->Halt();

	if(status == 0)
		currentThread->Finish();
	else
		printf("Cannot exit current thread\n");
	
}
#endif

//-----------------------------------------------------
//---------------- EXCEPTION HANDLER ------------------
//-----------------------------------------------------

void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2); // Which syscall?
    int rv=0; 	// the return value from a syscall

    if ( which == SyscallException ) {
	switch (type) {
	    default:
		DEBUG('a', "Unknown syscall - shutting down.\n");
	    case SC_Halt:
		DEBUG('a', "Shutdown, initiated by user program.\n");
		interrupt->Halt();
		break;
	    case SC_Create:
		DEBUG('a', "Create syscall.\n");
		Create_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
	    case SC_Open:
		DEBUG('a', "Open syscall.\n");
		rv = Open_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
	    case SC_Write:
		DEBUG('a', "Write syscall.\n");
		Write_Syscall(machine->ReadRegister(4),
			      machine->ReadRegister(5),
			      machine->ReadRegister(6));
		break;
	    case SC_Read:
		DEBUG('a', "Read syscall.\n");
		rv = Read_Syscall(machine->ReadRegister(4),
			      machine->ReadRegister(5),
			      machine->ReadRegister(6));
		break;
	    case SC_Close:
		DEBUG('a', "Close syscall.\n");
		Close_Syscall(machine->ReadRegister(4));
		break;

		// Project 2 part 1 addition
#ifdef USER_PROGRAM
		case SC_CreateLock:
		DEBUG('a', "CreateLock syscall.\n");
		rv = CreateLock_Syscall();
		break;
		case SC_DestroyLock:
		DEBUG('a', "DestroyLock syscall.\n");
		DestroyLock_Syscall(machine->ReadRegister(4));
		break;
		case SC_Acquire:
		DEBUG('a', "Acquire syscall.\n");
		Acquire_Syscall(machine->ReadRegister(4));
		break;
		case SC_Release:
		DEBUG('a', "Release syscall.\n");
		Release_Syscall(machine->ReadRegister(4));
		break;
		case SC_CreateCondition:
		DEBUG('a', "CreateCondition syscall.\n");
		rv = CreateCondition_Syscall();
		break;
		case SC_DestroyCondition:
		DEBUG('a', "DestroyCondition syscall.\n");
		DestroyCondition_Syscall(machine->ReadRegister(4));
		break;
		case SC_Signal:
		DEBUG('a', "Signal syscall.\n");
		Signal_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
		case SC_Wait:
		DEBUG('a', "Wait syscall.\n");
		Wait_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
		case SC_Broadcast:
		DEBUG('a', "Broadcast syscall.\n");
		Broadcast_Syscall(machine->ReadRegister(4), machine->ReadRegister(5));
		break;
		
		// Project 2 part 2 addition
		case SC_Fork:
		DEBUG('a', "Fork syscall.\n");
		Fork_Syscall(machine->ReadRegister(4));
		break;
		case SC_Exec:
		DEBUG('a', "Exec syscall.\n");
		// Exec_Syscall(machine->ReadRegister(4));
		break;
		case SC_Exit:
		DEBUG('a', "Exit syscall.\n");
		Exit_Syscall(machine->ReadRegister(4));
		break;
		case SC_Yield:
		DEBUG('a', "Yield syscall.\n");
		Yield_Syscall();
		break;
#endif
	}

	// Put in the return value and increment the PC
	machine->WriteRegister(2,rv);
	machine->WriteRegister(PrevPCReg,machine->ReadRegister(PCReg));
	machine->WriteRegister(PCReg,machine->ReadRegister(NextPCReg));
	machine->WriteRegister(NextPCReg,machine->ReadRegister(PCReg)+4);
	return;
    } else {
      cout<<"Unexpected user mode exception - which:"<<which<<"  type:"<< type<<endl;
      interrupt->Halt();
    }
}

