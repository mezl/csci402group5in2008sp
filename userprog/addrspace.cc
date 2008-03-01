// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "../threads/system.h"
#include "copyright.h"
#include "addrspace.h"
#include "noff.h"
#include "table.h"
#include "synch.h"

#ifndef PROJ2
#define PROJ2
#endif
extern "C" { int bzero(char *, int); };

Table::Table(int s) : map(s), table(0), lock(0), size(s) {
    table = new void *[size];
    lock = new Lock("TableLock");
}

Table::~Table() {
    if (table) {
	delete table;
	table = 0;
    }
    if (lock) {
	delete lock;
	lock = 0;
    }
}

void *Table::Get(int i) {
    // Return the element associated with the given if, or 0 if
    // there is none.

    return (i >=0 && i < size && map.Test(i)) ? table[i] : 0;
}

int Table::Put(void *f) {
    // Put the element in the table and return the slot it used.  Use a
    // lock so 2 files don't get the same space.
    int i;	// to find the next slot

    lock->Acquire();
    i = map.Find();
    lock->Release();
    if ( i != -1)
	table[i] = f;
    return i;
}

void *Table::Remove(int i) {
    // Remove the element associated with identifier i from the table,
    // and return it.

    void *f =0;

    if ( i >= 0 && i < size ) {
	lock->Acquire();
	if ( map.Test(i) ) {
	    map.Clear(i);
	    f = table[i];
	    table[i] = 0;
	}
	lock->Release();
    }
    return f;
}

//---------------------------------------------------------
//-------------------PROJECT 2 PART2-----------------------
//---------------PROCESS TABLE IMPLEMENTATION--------------
//---------------------------------------------------------
ProcessTable::ProcessTable()
{
	processTableLock = new Lock("Process Table Lock");
}

ProcessTable::~ProcessTable()
{
	delete processTableLock;
	std::map<int, std::vector<Thread*> >::iterator iter;
	iter = hashmap.begin();
	while(!(iter == hashmap.end()))
	{
		(iter->second).clear();
		iter++;
	}
	hashmap.clear();
}

int ProcessTable::AddThread(Thread* myThread)
{
	if(myThread == NULL)
	{
		return -1;
	}

	processTableLock->Acquire();
	hashmap[myThread->space->getSpaceID()].push_back(myThread);
	processTableLock->Release();
	return myThread->space->getSpaceID();
}
int ProcessTable::RemoveThread(Thread* myThread)
{
	if (myThread == NULL)
	{
		return -1;
	}

	processTableLock->Acquire();
	
	int mySpaceId = myThread->space->getSpaceID();
	int targetThreadFound = 0;
	unsigned int i = 0;
	int found = 0;

	std::vector<Thread*>::iterator iter;
	iter = hashmap[mySpaceId].begin();
	while(i < hashmap[mySpaceId].size() )
	{
		if(myThread == (*iter))
		{
			found = 1;
			hashmap[mySpaceId].erase(iter);
		}
		iter++;
		i++;
	}
	
	if(found == 0)
	{
		return -1;
	}

	if(hashmap.empty() == TRUE)
		return 1;
	if(hashmap[mySpaceId].empty())
	{
		hashmap.erase(mySpaceId);
		return 2;
	}


	processTableLock->Release();
	return 0;
}


int ProcessTable::CheckChildExist(int mySpaceId)
{
	if(hashmap[mySpaceId].empty())
	{
		return 1;
	}
	return 0;
}


//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	"executable" is the file containing the object code to load into memory
//
//      It's possible to fail to fully construct the address space for
//      several reasons, including being unable to allocate memory,
//      and being unable to read key parts of the executable.
//      Incompletely consretucted address spaces have the member
//      constructed set to false.
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable) : fileTable(MaxOpenFiles) {
    NoffHeader noffH;
    unsigned int i, size;
#ifdef PROJ2
	spaceLock = new Lock("AddrSpace lock");
#endif
    // Don't allocate the input or output to disk files
    fileTable.Put(0);
    fileTable.Put(0);

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size ;
    numPages = divRoundUp(size, PageSize) + divRoundUp(UserStackSize,PageSize);
                                                // we need to increase the size
						// to leave room for the stack
    size = numPages * PageSize;

    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
						// virtual memory

    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
	pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
#ifdef PROJ2
	int newpage = memoryTable.Put(0);//Find the new space in memory table
	//Just booking the space, but not put anything in to it yet
	if(newpage == -1){
		printf("Can't not creat new addr space, no more memory");
		return;
	}	
	pageTable[i].physicalPage = newpage;
#else
/////////////////////////////////////////////////////////////////////////////
	pageTable[i].physicalPage = i;
#endif
/////////////////////////////////////////////////////////////////////////////
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
int mainMemAddr = pageTable[i].physicalPage * PageSize;
bzero(&(machine->mainMemory[mainMemAddr]), PageSize);
    }
    
//---------------------------------------------------------------------------//
#ifdef PROJ2
//Doing the memory load from the user code and data seg.
//-----
//|   |
//|   |
//|   |
//|   |
//-----
int mainMemAddr = 0; 
int fileMemAddr = 0; 
int currentBytes = 0;
int notReadBytes = 0;
itsSpaceID = pageTable[0].physicalPage;
itsMaxForkAddr = noffH.code.size -1;//The last address fork can go
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);

		//Set start & end address range
		//Then we need divide the code to several page
		//They will load to different place
		notReadBytes = noffH.code.size;
		currentBytes = 0;
		
		//Start to load data to each page
		for(i = 0; notReadBytes >= PageSize; i++){
				mainMemAddr = pageTable[i].physicalPage * PageSize;
				fileMemAddr = noffH.code.inFileAddr + currentBytes;
				//Only load one page size;
				executable->ReadAt(&(machine->mainMemory[mainMemAddr]), PageSize, fileMemAddr);

				currentBytes += PageSize;
				notReadBytes -= PageSize;
		}

		//The last bytes is less than a page size, but we still need allocated
		//A space for those data
		if(notReadBytes > 0)
		{
				mainMemAddr = pageTable[i].physicalPage * PageSize;
				fileMemAddr = noffH.code.inFileAddr + currentBytes;
				//Only load left memory size;
				executable->ReadAt(&(machine->mainMemory[mainMemAddr]), notReadBytes, fileMemAddr);
				
				//Now we read all the code in to memory
				//currentBytes += notReadBytes; //currentBytes equal noffH.code.size now
				//notReadBytes = 0;// notReadBytes is zero
		}

    }//finish noffH.code.size load

	//Now we will start to handle to initData
	//If the last page from prev page still have some space
	//We will also fill that unuse space before we move to next page
	
	int lastFreeSpaceSize  = PageSize - notReadBytes;

    if (noffH.initData.size > 0) 
	{
			DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
							noffH.initData.virtualAddr, noffH.initData.size);
			currentBytes = 0;//reset current read location

			//Handle the last free space	
			if(notReadBytes > 0)
			{
					//Check the initData is even samller than the last free space			
					if(noffH.initData.size < lastFreeSpaceSize)
							lastFreeSpaceSize = noffH.initData.size;							

					mainMemAddr = pageTable[i].physicalPage * PageSize + notReadBytes;
					fileMemAddr = noffH.initData.inFileAddr ;
					executable->ReadAt(&(machine->mainMemory[mainMemAddr]), lastFreeSpaceSize, fileMemAddr);
					currentBytes = lastFreeSpaceSize;
			}
			i++;//Move to next empty page
			//Now we may already some of initData
			//So we start count the current initData location
			notReadBytes = noffH.initData.size - currentBytes;

			//Keep going don't need reset page index
			for(;notReadBytes >= PageSize;i++)
			{
					mainMemAddr = pageTable[i].physicalPage * PageSize ;
					fileMemAddr = noffH.initData.inFileAddr + currentBytes;
					executable->ReadAt(&(machine->mainMemory[mainMemAddr]), PageSize, fileMemAddr);
					currentBytes += PageSize;
					notReadBytes -= PageSize;
			}
			//The last part for initData
			if(notReadBytes > 0)	
			{
					mainMemAddr = pageTable[i].physicalPage * PageSize ;
					fileMemAddr = noffH.initData.inFileAddr + currentBytes;
					executable->ReadAt(&(machine->mainMemory[mainMemAddr]), notReadBytes, fileMemAddr);
			}
			// now currentBytes + notReadbytes should == initData.size
			// And notReadbytes will be zero;

	}
 
#else
//---------------------------------------------------------------------------//

// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    bzero(machine->mainMemory, size);
// then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }
#endif
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//
// 	Dealloate an address space.  release pages, page tables, files
// 	and file tables
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    delete pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %x\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
#ifdef PROJ2
int AddrSpace::getSpaceID()
{
	return itsSpaceID;
}
int AddrSpace::getMaxForkAddr()
{
	return itsMaxForkAddr;
}
int AddrSpace::newStack()
{
		spaceLock->Acquire();
		unsigned int newNumPages =  divRoundUp(UserStackSize,PageSize);
		TranslationEntry *newTable;
		//int stackAddr[newNumPages];
		newTable = new TranslationEntry[newNumPages + numPages];
		
		for(unsigned int i = 0 ;i < numPages+newNumPages ; i++)
		{
				if(i<numPages){
						newTable[i].physicalPage = pageTable[i].physicalPage ;
						newTable[i].valid        = pageTable[i].valid ;
						newTable[i].use          = pageTable[i].use ;
						newTable[i].dirty        = pageTable[i].dirty ;
						newTable[i].readOnly     = pageTable[i].readOnly ; 
				}else{
						newTable[i].physicalPage = memoryTable.Put(0);
						if(newTable[i].physicalPage == -1){
								printf("Not Enough Memory Space!\n");
								return 0;
						}
							
						newTable[i].valid        = TRUE; 
						newTable[i].use          = FALSE;
						newTable[i].dirty        = FALSE;
						newTable[i].readOnly     = FALSE;

						int mainMemAddr = newTable[i].physicalPage * PageSize;
						bzero(&(machine->mainMemory[mainMemAddr]), PageSize);

				}

		}
	//Replace to the new Table
	
	pageTable->~TranslationEntry();
	pageTable = newTable;
	numPages+=newNumPages;
	spaceLock->Release();
	return numPages*PageSize -16 ;

}

#endif

