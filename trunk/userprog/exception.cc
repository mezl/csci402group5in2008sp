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
    int n=0;			// The number of bytes copied in
    
#ifdef PROJZ
   unsigned int vpn = vaddr /PageSize;
   unsigned int ppn;
   unsigned int offset = vaddr % PageSize;
#else   
    bool result;
    int *paddr = new int;
#endif   
    while ( n >= 0 && n < len) {
#ifdef PROJZ
      if(vpn< machine->pageTableSize && machine->pageTable[vpn].valid)
      {
         ppn = machine->pageTable[vpn].physicalPage;
         if(ppn < NumPhysPages){
            buf[n] = nachine->mainMemory[ppn *PageSize + offset];
            if(buf[n++] == '\0')
               break;
         }else{
            n = -1;
         }
      }else{
         n = -1;
      }
      vaddr++;
     }//end while
     return n;
#else      
      result = machine->ReadMem( vaddr, 1, paddr );
#ifdef PROJ3
      //printf("[copyin]\n");
      while(!result){
         result = machine->ReadMem( vaddr, 1, paddr );
      }
#else
      if ( !result ) {
	//translation failed
	return -1;
      }
#endif
      buf[n++] = *paddr;
      vaddr++;
    }//end while

    delete paddr;
    return len;
#endif   
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

#ifdef PROJ3
      //printf("[copyout]\n");
      while(!result)
         result = machine->WriteMem( vaddr, 1, (int)(buf[n++]) );
#else         
      if ( !result ) {
	//translation failed
	return -1;
      }

#endif    
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
	    printf("Bad pointer[%d]passed to to write: data not written\n",vaddr);
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
		;//printf("Unable to destroy Lock because not last child thread \n");
	
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
		;//printf("Unable to destroy Condition because not last child thread \n");
	
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
	if(myFile == NULL)
	{
		printf("Failure on Exec System Call: openFile doesn't exist \n");
		return -1;
	}
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
#ifdef PROJ3
	if(virtualaddress > currentThread->space->getCodeSize())
	{
		printf("Failure to access fork address %d\n",virtualaddress);
		return;
	}
#endif
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

	if(check == 2)//if it's last thread and last process
		interrupt->Halt();

	if(status == 0)
		currentThread->Finish();
	else
		printf("Cannot exit current thread\n");

}
#endif

#ifdef PROJ3
#define HAVE_IPT
//------Pure TLB FUNCTION----------
int getFreeTLBSlot(void);
int getTLBSlot(bool *dirty);
int getNextEvictTLBSlot(void);
void TLB_Update(int ppn,int vpn);
void updateTLBEntry(int tlb_slot,int ppn,int vpn);

//------IPT & TLB FUNCTION----------
#ifdef HAVE_IPT
void TLB_Update(int ppn);
void updateTLBEntry(int tlb_slot,int ppn);
void propagateToIPT(int tlb_slot);
#endif
//------IPT FUNCTION----------
#ifdef HAVE_IPT
int getTLB_Slot(int ipt_slot);
void IPTSwapToMemory(int ipt_slot,int vpn);
void IPTSwapToFile(int ipt_slot);
int getNextEvictIPTSlot(void);
int getFreeIPTSlot(void);
int getIPT_Slot(void);
void updateIPTEntry(int ipt_slot,int vpn);
int IPT_Update(int vpn);
int IPTHit(int pid,int vpn);
#endif
//------Util FUNCTION----------
int getPhysAddr(int vpn);
//-----------------------------------------------------
//---------------- Page Fault HANDLER -----------------
//-----------------------------------------------------
void PageFault_Handler(unsigned int va)
{
	IntStatus old = interrupt->SetLevel(IntOff);
   unsigned int vpn = va/PageSize;	
   if(currentThread->space !=NULL){
      unsigned int cnpg = currentThread->space->getNumPages();
      //printf("CurrentThread numPages %d\n",cnpg);
      if(vpn >=cnpg){
         printf("illeagle virtual address %d",vpn);
         interrupt->Halt();
      }
   }
	//Acquire the memory lock
   //Bug Here!
	int ppn = getPhysAddr(vpn); 
#ifdef HAVE_IPT   
   //int pid = currentThread->space->getProcessID();
	//int IPTFound = IPTHit(pid,vpn);
   
	int IPTFound = IPTHit(ppn,vpn);
	if(IPTFound == -1){
		//Not in IPT
		IPTFound = IPT_Update(vpn);
	}
	TLB_Update(IPTFound);
#else
	TLB_Update(ppn,vpn);
#endif
	interrupt->SetLevel(old);
	//Release the memory lock

}

int getPhysAddr(int vpn)
{
	//VmTranslationEntry* pt = (IPTable[ipt_slot].space)->getPageTable();
	VmTranslationEntry* pt = currentThread->space->getPageTable();
	//int ppn = currentThread->space->getSpaceID();
	int ppn = pt[vpn].physicalPage;
   //printf("[getPhysAddr]ppn = %d \n",ppn);
   //pt->physicalPage; 
   return ppn;
      
}

#ifdef HAVE_IPT
//-----------------------------------------------------
//---------------- IPT Function------------------------
//-----------------------------------------------------

//---------------- IPT Hit-----------------------------
//Check the address is in IPT or not
//If IPThit is true,return physical address
//otherwire return -1
//Pre:
//	pid : proccess ID
//	vpn : virtual page number
//Post: 
//Return: IPT Hit True: physical page number 
//		  IPT Hit False: -1
int IPTHit(int pid,int vpn)
{
	for(int i = 0; i < NumPhysPages; i++)
	{
		if(IPTable[i].valid == TRUE)
		{
			if(IPTable[i].pid == pid && IPTable[i].vpn == vpn){
				return i;
			}
		}
	}
	return -1;
}

//---------------- IPT Update-----------------------------
//Insert Current Physical Address in to the IPT table
//Pre:
//	vpn: virtual page number
//Post:
//	[None]
//Return:
//	Physical Page Number		
int IPT_Update(int vpn)
{
	//OpenFile *current_exec = currentThread->space->get
	int ipt_slot = getIPT_Slot();
	int tlb_slot; 
	if(IPTable[ipt_slot].valid){
		tlb_slot = getTLB_Slot(ipt_slot);
		if(tlb_slot != -1){
			IPTable[ipt_slot].dirty = machine->tlb[tlb_slot].dirty;
			machine->tlb[tlb_slot].valid = FALSE;
		}
		if(IPTable[ipt_slot].dirty){
			IPTSwapToFile(ipt_slot);
		}
		IPTSwapToMemory(ipt_slot,vpn);
	}
	updateIPTEntry(ipt_slot,vpn);			
	return ipt_slot;
}
void updateIPTEntry(int ipt_slot,int vpn)
{
//	VmTranslationEntry* pt = currentThread->space->getPageTable();
//	int ppn = pt[ipt_slot].physicalPage;
	IPTable[ipt_slot].pid = getPhysAddr(vpn);
   //currentThread->space->getProcessID();
	IPTable[ipt_slot].vpn= vpn; 
	IPTable[ipt_slot].space = currentThread->space;
	IPTable[ipt_slot].use = TRUE;
	IPTable[ipt_slot].valid = TRUE;
	IPTable[ipt_slot].readOnly = FALSE;
   printf("[updateIPTEntry] ipt_slot %d \n",ipt_slot);
   printf("[updateIPTEntry] pid %d vpn %d \n",IPTable[ipt_slot].pid,
         IPTable[ipt_slot].vpn);
}
int getIPT_Slot(void)
{
	int ipt_slot = getFreeIPTSlot();
	if(ipt_slot == -1)//no free slot in IPT,need evict some thing
	{
		switch(ipt_replace_algorithm)
      {
			case FIFO:
				ipt_slot = getNextEvictIPTSlot(); 
				break;
			case RAND:
				ipt_slot = rand()%NumPhysPages;
				break;
			default:
            printf("Can't find IPT replace algorithm\n");
            break;
		}

	}
   //printf("[getIPT_Slot] slot is %d \n",ipt_slot);
	return ipt_slot;
}

//---------------- IPT Free Spot-----------------------------
//Check the any free spot in the IPT 
//If some of IPT is invalid,we return this spot 
//otherwire return -1
//Pre:
//Post: 
//Return: IPT Have Free Spot: free spot number 
//		  IPT No Free Spot: -1
int getFreeIPTSlot(void)
{
	for(int i = 0; i< NumPhysPages; i++){
		if(IPTable[i].valid == FALSE)
			return i;
	}
	return -1;
}
//---------------- Evict IPT Slot -------------------------
//Return: Next IPT spot count 
//Note the int nextEvictIPTSlot is a global variable
int getNextEvictIPTSlot(void)
{
   nextEvictIPTSlot++;
   nextEvictIPTSlot%=NumPhysPages;
   //printf("nextEvictIPTSlot = %d\n",nextEvictIPTSlot);
	return (nextEvictIPTSlot);
}

//---------------- Write to Swap-------------------------
//The Acutal Memory to Swap Handle is done by AddrSpace
//Pre: ipt slot which wrtie to swap
//Post: save swap address to the PageTable Entry 
//		change the location to the PageTable Entry
//Return: None
void IPTSwapToFile(int ipt_slot)
{
	VmTranslationEntry* pt = (IPTable[ipt_slot].space)->getPageTable();
	int swapAddr = (IPTable[ipt_slot].space)->toSwap(ipt_slot);
	pt[IPTable[ipt_slot].vpn].location = SWAP;
	pt[IPTable[ipt_slot].vpn].swapAddr = swapAddr;
}
//---------------- Read Swap to Memory-----------------------
//Check the page table to find the location of those data
//if the it's in the executable file, we ask AddrSpace to bring
//it to memory,othwise we read from swapfile to memory and clear the swapMap
//Pre: ipt slot which want to read from swap
//Post: ipt data read into memory 
//Return: None
void IPTSwapToMemory(int ipt_slot,int vpn)
{
	VmTranslationEntry* pt = currentThread->space->getPageTable();
	switch(pt[vpn].location){
		case EXEC:
			currentThread->space->readExec(ipt_slot,vpn);
			IPTable[ipt_slot].dirty = FALSE;
			break;
		case SWAP:
			swapfile->ReadAt(
			&(machine->mainMemory[ipt_slot*PageSize]),
			PageSize,
			pt[vpn].swapAddr*PageSize
			);
			swapFileMap->Clear(pt[vpn].swapAddr);
			IPTable[ipt_slot].dirty = TRUE;
         break;
		default:
         break;	
	}
}
#endif
//-----------------------------------------------------
//---------------- TLB Function------------------------
//-----------------------------------------------------

#ifdef HAVE_IPT
//---------------- get TLB slot------------------------
//get TLB slot by given IPT slot
//Pre:
//	ipt_slot: ipt slot for search 
//Post:
//	[None]
//Return:
//	If found, then return the TLB slot which map to the IPT slot
//  Else return -1 
int getTLB_Slot(int ipt_slot){
	IPTEntry c = IPTable[ipt_slot];
	if(c.valid ==TRUE)
		if(currentThread->space == c.space)
			for(int i = 0;i < TLBSize ; i++)
				if(machine->tlb[i].valid == TRUE)
					if(machine->tlb[i].virtualPage == c.vpn)
						return i;
	return -1;
}

#endif
//---------------- TLB Update-----------------------------
//When Calling the TLB Update
//it will find the TLB slot to store the data
//if the slot is dirty, it will propagate to IPT
//the write the virtual and physical page data to it 
//Pre:
//	ppn: physical page number 
// vpn: virtual page number(if w/o useing IPT)
//Post:
//	tlb table updated	
//Return:
//	[None]
//---------------- TLB Update with IPT----------------------
#ifdef HAVE_IPT
void TLB_Update(int ppn)
{
   //printf("[TLB_Update]Using IPT ,ppn %d\n",ppn);
   
	bool dirty;
	int tlb_slot = getTLBSlot(&dirty);
	if(dirty)
		propagateToIPT(tlb_slot);
	updateTLBEntry(tlb_slot,ppn);	
}
#endif
//---------------- TLB Update no IPT----------------------
void TLB_Update(int ppn,int vpn)
{
   //printf("[TLB_Update]No IPT,ppn %d,vpn %d\n",ppn,vpn);
	bool dirty;
	int tlb_slot = getTLBSlot(&dirty);
	updateTLBEntry(tlb_slot,ppn,vpn);	
}
//---------------- Update TLB Entry-----------------------
//Calling by the TLB Update
//the write the virtual and physical page data to TLB
//Since we have two update method
// 1. UpdateTLB Entry without IPT table:
//       in this case, we need have 3 parameter 
//         tlb_slot,physical page number,virtual page number
//       Then default dirty bit will set false
//
// 2. UpdateTlB Entry with IPT
//       Then we only need tlb_slot and physical page number
//       We will get vpn by the IPT table,then copy the dirty bit
//       data from IPT table.
//Pre:
// tlb_slot: which slot we need update in TLB
//	ppn: physical page number 
// vpn: virtual page number(if w/o useing IPT)
//
//Post:
//	tlb table updated	
//Return:
//	[None]
void updateTLBEntry(int tlb_slot,int ppn,int vpn)
{
   //printf("[updateTLBEntry]No IPT,slot %d,ppn %d,vpn %d\n",tlb_slot,ppn,vpn);
	machine->tlb[tlb_slot].virtualPage = vpn;
	machine->tlb[tlb_slot].physicalPage = ppn;
	machine->tlb[tlb_slot].use = TRUE;
	machine->tlb[tlb_slot].valid = TRUE;
	machine->tlb[tlb_slot].dirty = FALSE;
	machine->tlb[tlb_slot].readOnly;
}
#ifdef HAVE_IPT
void updateTLBEntry(int tlb_slot,int ppn)
{
   //printf("[updateTLBEntry]With IPT,slot %d,ppn %d,vpn %d\n",tlb_slot,ppn,IPTable[ppn].vpn);
	machine->tlb[tlb_slot].virtualPage = IPTable[ppn].vpn;
	machine->tlb[tlb_slot].physicalPage = ppn;
	machine->tlb[tlb_slot].use = TRUE;
	machine->tlb[tlb_slot].valid = TRUE;
	machine->tlb[tlb_slot].dirty = IPTable[ppn].dirty;
	machine->tlb[tlb_slot].readOnly;
}
void propagateToIPT(int tlb_slot)
{
	int pp = machine->tlb[tlb_slot].physicalPage;
	int vp = machine->tlb[tlb_slot].virtualPage;
	int cpid = currentThread->space->getProcessID();
	if(IPTable[pp].pid == cpid && IPTable[pp].vpn == vp)
	{
		IPTable[pp].dirty = machine->tlb[tlb_slot].dirty;
	}else{
		printf("IPT Error\n");
		interrupt->Halt();
	}
}
#endif
int getNextEvictTLBSlot(void)
{
   nextEvictTLBSlot++;
   nextEvictTLBSlot%= TLBSize;
	return nextEvictTLBSlot;
}
int getTLBSlot(bool *dirty)
{
	int tlb_slot = getFreeTLBSlot();
	if(tlb_slot == -1){
		tlb_slot = getNextEvictTLBSlot();
		*dirty = machine->tlb[tlb_slot].dirty;
	}else{
		*dirty = FALSE;
	}
	
	return tlb_slot;//tlb can be dirty ,may need propagate to IPT
}
int getFreeTLBSlot(void)
{
	for(int i = 0; i < TLBSize ; i++){
		if(machine->tlb[i].valid == FALSE){
			return i;
		}
	}
	return -1;
}

#endif
//-----------------------------------------------------
//---------------- EXCEPTION HANDLER ------------------
//-----------------------------------------------------

void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2); // Which syscall?
	int rv=0; 	// the return value from a syscall
	bool reading;
	int tempo;
	int vaddress;
	char name[20];
	int i;

	if ( which == SyscallException ) 
	{
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

				i=0;
				reading = machine->ReadMem(machine->ReadRegister(4), 1, &tempo);
				vaddress = machine->ReadRegister(4);
				while(reading && !(tempo==0))
				{
					name[i] = tempo;
					vaddress++;
					reading = machine->ReadMem(vaddress, 1, &tempo);
					i++;
				}
				if(!reading)
					rv=-1;
				else
					rv =  Exec_Syscall(name);

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
#ifdef PROJ3
	} else if(which == PageFaultException){
		PageFault_Handler(machine->registers[BadVAddrReg]);
		return;
#endif	
	} else {
		cout<<"Unexpected user mode exception - which:"<<which<<"  type:"<< type<<endl;
		interrupt->Halt();
	}
}

