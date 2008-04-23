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
#include "../network/post.h"
#include "network.h"
#include <stdlib.h>
//#define RPC_DEBUG
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

// -------------------------------------------------------
// -------------- PROJECT 3 (part 3) ---------------------
// -------------------------------------------------------
#ifdef NETWORK
class PacketHeader;
class MailHeader;
PacketHeader outPktHdr, inPktHdr;
MailHeader outMailHdr, inMailHdr;
char buffer[15];
bool success;
char outMessage[15];
char inMessage[15];
char tempMessage[15];

void ClearArray(char name[15])
{
   for(int i=0; i<15; i++)
      name[i] = ' ';
}

void ClientSendReceiveRequest(int port)
{
   outPktHdr.to = 0; //send to machine 0, which is the server
   outMailHdr.to = 0;
   outMailHdr.from = port;//port;
   outMailHdr.length = strlen(outMessage)+1;
   success = postOffice->Send(outPktHdr, outMailHdr, outMessage);
   printf("[CLIENT]Sending a packet of \"%s\" to %d, box %d\n", outMessage, outPktHdr.to, outMailHdr.to);


   if(!success)
   {
      printf("[CLIENT]Send failed. Probably don't have server running. Terminating Nachos\n");
      interrupt->Halt();
   }

   printf("[CLIENT]Client waiting for response...\n");
   postOffice->Receive(port, &inPktHdr, &inMailHdr, buffer);
   strcpy(inMessage, buffer);
   printf("[CLIENT]Client got \"%s\" from %d, box %d\n", inMessage, inPktHdr.from,inMailHdr.from);
   fflush(stdout);
}
void ClientSendReceiveRequest(){
   ClientSendReceiveRequest(1);
}

// -------------------------------------------------------
// -------------- PROJECT 4 (part 1) ---------------------
// -------------------------------------------------------

//-------------getRandServerID()-------------------------
//Generate the random server ID
int server_count = 0;
int getRandServerID()
{
   server_count += rand()%10;
   return (server_count++)%SERVER_NUM;
}

//-------------SR_server(int port,char *outMsg,char *inMsg)--------
//Send a message to server and get one message back
//Pre:sender's port number, outgoing message
//Post:the server response message
//Return:none
void SR_server(int port,char *outMsg,char *inMsg)
{
   PacketHeader SR_outPktHdr, SR_inPktHdr;
   MailHeader SR_outMailHdr, SR_inMailHdr;
   int result;
   char SRbuf[256];
   SR_outPktHdr.to = getRandServerID(); //send to machine 0, which is the server
   SR_outMailHdr.to = 0;
   SR_outMailHdr.from = port;//port;
   SR_outMailHdr.length = strlen(outMsg)+1;
   result = postOffice->Send(SR_outPktHdr, SR_outMailHdr, outMsg);
   printf("[CLIENT]Sending a packet of \"%s\" to server [%d], box %d\n", outMsg,
   SR_outPktHdr.to, SR_outMailHdr.to);
#ifdef RPC_DEBUG   
#endif   


   if(!result)
   {
      printf("[CLIENT]Send failed. Probably don't have server running. Terminating Nachos\n");
      interrupt->Halt();
   }

#ifdef RPC_DEBUG   
   printf("[CLIENT]Client waiting for response...\n");
#endif   
   postOffice->Receive(port, &SR_inPktHdr, &SR_inMailHdr, SRbuf);
   strcpy(inMsg, SRbuf);
#ifdef RPC_DEBUG   
   printf("[CLIENT]Client got \"%s\" from %d, box %d\n", inMsg, SR_inPktHdr.from,SR_inMailHdr.from);
#endif   
   fflush(stdout);


}
#endif

//#ifdef USER_PROGRAM
#ifdef NETWORK
int CreateLock_Syscall(int vaddr)
#else
int CreateLock_Syscall()
#endif
{
#ifdef NETWORK
   int lockID;
   char temp[15];
   ClearArray(temp);
   char tempName[15];

   printf("====Start CreateLock_Syscall====\n");

   char *lockName = new char[MAXFILENAME];
   if(!lockName){
      printf("==CreateLock_Syscall==Can't allocate open lockName space\n");
      return -1;
   }
   if(copyin(vaddr,MAXFILENAME,lockName) == -1)
   {
      printf("==[CreateLock_Syscall]Can't open the file\n");
   }
   sscanf(lockName, "%s", tempName);
   ClearArray(outMessage);
   sprintf(outMessage, "LC %s\0", tempName);
   ClientSendReceiveRequest(); //send the RPC and gather the response

   sscanf(inMessage, "%s %d", temp, &lockID);
   if(strcmp(temp, "LCS") != 0)
   {
      printf("[CLIENT]Creating a Lock %s failed\n", tempName);
      return -1;
   }
   printf("[CLIENT]Creating a Lock %s successful\n", tempName, lockID);
   return lockID;

#else
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
#endif
}

void DestroyLock_Syscall(int id)
{
#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "LD %d\0", id);
   ClientSendReceiveRequest(); // send request and gather response
   if(strcmp(inMessage, "LDS") != 0)  // destroy lock failed
   {
      printf("[CLIENT]Lock %d fail to be destroyed\n", id);
      return;
   }
   printf("[CLIENT]Lock successfully destroyed \n");

#else
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
#endif
}

void Acquire_Syscall(int id)
{

   printf("====Start Acquire_Sycall====\n");
#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "LA %d\0", id);
   ClientSendReceiveRequest();

   if(strcmp(inMessage, "LAF") == 0)
   {
      printf("[CLIENT]Lock fail to be Acquired\n");
      return;
   }
   
   //While waiting to acquiring lock
   //Keep Checking the network package until get the message for
   //LAS(Lock Acquire Success)
   while(strcmp(inMessage,"LAS") != 0)
   {
      printf("[CLIENT]Someone is using the lock, need wait\n");
      postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
      strcpy(inMessage, buffer);
      printf("[CLIENT]Client got \"%s\" from %d, box %d\n", inMessage, inPktHdr.from, inMailHdr.from);
      fflush(stdout);
      if(strcmp(inMessage, "LAS") == 0)
      {
         printf("[CLIENT]Client has get Lock now! \n");

         return;
      }
   }
   printf("[CLIENT]Lock %d successfully acquired\n");
#else


   Lock* myLock = (Lock*)lockTable.Get(id);

   if(myLock == NULL)
   {
      printf("Failure Acquiring a lock of index %d \n", id);
      return;
   }
   else{
      myLock -> Acquire();}
#endif
}

void Release_Syscall(int id)
{

#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "LR %d\0", id);
   ClientSendReceiveRequest();
   if(strcmp(inMessage, "LRS") != 0)
   {
      printf("Lock %d fail to be released \n", id);
      return;
   }
   printf("[CLIENT]Lock %d successfully released \n", id);

#else

   printf("====Start Release_Syscall====\n");

   Lock* myLock = (Lock*)lockTable.Get(id);

   if(myLock == NULL)
   {
      printf("Failure Releasing a lock %d \n", id);
      return;
   }
   else{
      myLock -> Release();}
#endif
}

#ifdef NETWORK
int CreateCondition_Syscall(int vaddr)
#else
int CreateCondition_Syscall()
#endif
{
#ifdef NETWORK

   int condID;
   char temp[15];
   ClearArray(temp);
   char tempName[15];

   printf("====Start CreateCondition_Sycall====\n");

   char *condName = new char[MAXFILENAME];
   if(!condName){
      printf("==CreateCond_Syscall==Can't allocate open condName space\n");
      return -1;
   }
   if(copyin(vaddr,MAXFILENAME,condName) == -1)
   {
      printf("==[CreateCond_Syscall]Can't open the file\n");
   }
   sscanf(condName, "%s", tempName);
   ClearArray(outMessage);
   sprintf(outMessage, "CC %s\0", tempName);
   ClientSendReceiveRequest();

   sscanf(inMessage, "%s %d", temp, &condID);
   if(strcmp(temp, "CCS") != 0)
   {
      printf("[CLIENT]Creating a Condition %s failed\n", tempName);
      return -1;
   }
   printf("[CLIENT]Creating a condition %s successful\n", tempName, condID);
   return condID;
#else
   Condition* myCondition = new Condition("New Condition");
   int id = conditionTable.Put(myCondition);

   if(id < 0)
   {
      printf("Failure creating a condition of index %d \n", id);
      delete myCondition;
   }
   return id;
#endif
}

void DestroyCondition_Syscall(int id)
{	
#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "CD %d\0", id);
   ClientSendReceiveRequest();
   if(strncmp(inMessage, "CDS", 6) != 0)
   {
      printf("[CLIENT]Condition %d fail to be destroyed\n", id);
      return;
   }
   printf("Condition successfully destroyed \n");

#else
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

#endif
}

void Signal_Syscall(int lockID, int conditionID)
{

#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "CS %d %d\0", lockID, conditionID);
   ClientSendReceiveRequest();
   if(strcmp(inMessage, "CSS") != 0)
   {
      printf("[CLIENT]Condition %d fail to be signaled\n", conditionID);
      return;
   }
   printf("[CLIENT]Condition %d successfully signaled \n", conditionID);

#else

   printf("====Start Signal_Sycall====\n");

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
#endif
}

void Wait_Syscall(int lockID, int conditionID)
{

#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "CW %d %d\0", lockID, conditionID);
   ClientSendReceiveRequest();
   if(strcmp(inMessage, "CWF") == 0)
   {
      printf("[CLIENT]Condition %d fail to be waited\n", conditionID);
      return;
   }
   printf("[CLIENT]Condition %d successfully waited \n", conditionID);
   printf("[CLIENT]Client now goes into sleep.. \n");

   while(true)
   {
      postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
      strcpy(inMessage, buffer);
      printf("[CLIENT]Client got \"%s\" from %d, box %d\n", inMessage, inPktHdr.from, inMailHdr.from);
      fflush(stdout);
      if(strcmp(inMessage, "UP") == 0)
      {
         printf("[CLIENT]Client has been waken up! \n");
         return;
      }
   }

#else

   printf("====Start Wait_Sycall====\n");

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
#endif
}

void Broadcast_Syscall(int lockID, int conditionID)
{

#ifdef NETWORK

   ClearArray(outMessage);
   sprintf(outMessage, "%s", "CB ");
   sprintf(tempMessage, "%d", lockID);
   strcat(outMessage, tempMessage);
   sprintf(tempMessage, "%d", conditionID);
   strcat(outMessage, ".");
   strcat(outMessage, tempMessage);
   ClientSendReceiveRequest();
   while(strncmp(inMessage, "CBS", 6) != 0)
   {
      printf("Condition fail to be broadcasted, requesting service again \n");
      ClientSendReceiveRequest();
   }
   printf("Condition successfully broadcasted \n");

#else

   printf("====Start BroadCast_Sycall====\n");

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
#endif
}

//#endif

// ----------------------------------------------------
// --------------- PROJECT 2 PART 2 -------------------
// ----------------------------------------------------

//#ifdef USER_PROGRAM

void exec_thread()
{
   currentThread->space->InitRegisters();
   currentThread->space->RestoreState();
   machine->Run();
}
SpaceId Exec_Syscall(int va)
{
   printf("====Start Exec_Sycall====\n");

   char *name = new char[MAXFILENAME];
   if(!name){
      printf("==Exec_Syscall==Can't allocate open name space\n");
      return -1;
   }
   if(copyin(va,MAXFILENAME,name) == -1)
   {
      printf("==[Exec_Syscall]Can't open the file\n");
   }

   OpenFile* myFile = fileSystem->Open(name);
   if(myFile == NULL)
   {
      printf("Failure on Exec System Call: openFile doesn't exist \n");
      return -1;
   }
   AddrSpace* mySpace = new AddrSpace(myFile);
   Thread* myThread = new Thread(name);
   myThread->space = mySpace;
   int mySpaceId = processTable.AddThread(myThread);
   //myThread->space->setSpaceID(mySpaceId);
   //int myThreadId = processTable.AddThread(myThread);
   printf("===Process with <<<pid:%d >>>space now is Created!===\n",mySpaceId);	
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
   printf("====Start Fork_Sycall====\n");
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
   int processId = processTable.AddThread(myThread);

   printf("====Fork_Sycall add one more thread in <<pid:%d>>====\n",processId);
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
   printf("====Start Exit_Sycall with value [ %d ]====\n",status);
   /*
      if(processTable.CheckChildExist(currentThread->space->getSpaceID()) == 1)
      {
      currentThread -> Sleep();
      }*/

   // removeSuccessful = 0  : successful found thread and remove it 
   //                         not last thread in process
   // removeSuccessful = -1 : no found thread
   // removeSuccessful = 1  : removing last thread in a process,
   //                         but still have the other process running   
   // removeSuccessful = 2  : removing last thread in last process 


   int check = processTable.RemoveThread(currentThread);

   if(check == -1)
   {
      printf("Can't exit current thread\n");
   }
   if(check == 1 || check == 2)
      currentThread->space->~AddrSpace();
   if(check == 2){//if it's last thread and last process
      interrupt->Halt();
   }
   if(check == 0 || check == 1 || status == 0)
      currentThread->Finish();
   else{
      printf("Cannot exit current thread\n");
   }
}
//#endif

#ifdef PROJ3
#define HAVE_IPT
//#define TLB_DEBUG
//#define IPT_DEBUG 
//#define IPT_DEBUG_UPDATE 
//#define IPT_DEBUG_SWAP_OUT 
//#define IPT_DEBUG_SWAP_IN 
//#define PGF_DEBUG 
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
void IPTSwapToFile(int ipt_slot,int vpn);
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
#ifdef PGF_DEBUG
   printf("[PageFault_Handler] Start Page Fault with vaddr %d\n",va);
#endif
   physMemoryLock->Acquire();
   IntStatus old = interrupt->SetLevel(IntOff);
   unsigned int vpn = va/PageSize;	
   unsigned int cnpg = currentThread->space->getNumPages();
#ifdef PGF_DEBUG
   printf("[PageFault_Handler] vpn %d pageSize %d cnpg\n",vpn,PageSize,cnpg);
#endif
   //printf("CurrentThread numPages %d\n",cnpg);
   if(vpn >=cnpg){
      printf("illeagle virtual address %d\n",vpn);
      interrupt->Halt();
   }
   //Acquire the memory lock
   //Bug Here!
   //int ppn = getPhysAddr(vpn); 
#ifdef PGF_DEBUG
   //printf("[PageFault_Handler] vpn %d ppn %d pageSize %d\n",vpn,ppn,PageSize);
#endif
#ifdef HAVE_IPT   
   int pid = currentThread->space->getProcessID();
   int IPTFound = IPTHit(pid,vpn);

   //int IPTFound = IPTHit(ppn,vpn);
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
   physMemoryLock->Release();
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
#ifdef IPT_DEBUG
   //printf("[IPTHit] Start search IPT which pid %d vpn %d\n",pid,vpn);
#endif
   for(int i = 0; i < NumPhysPages; i++)
   {
      //printf("[IPTHit] Search IPT slot %d while valid is [%s]\n",
      //   i,IPTable[i].valid ? "True":"False");


      if(IPTable[i].valid == TRUE)
      {
         //printf("[IPTHit] IPT slot %d is valid, pid %d vpn %d\n",
         //   i,IPTable[i].pid,IPTable[i].vpn);
         if(IPTable[i].pid == pid && IPTable[i].vpn == vpn){
#ifdef IPT_DEBUG
            //       printf("[IPTHit] = IPT Hit! =Found ipt pid %d vpn %d in IPTslot %d\n"
            //             ,pid,vpn,i);
#endif
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
#ifdef IPT_DEBUG_UPDATE
      printf("[IPT_UPDATE]This slot %d have data in it\n");
#endif
      tlb_slot = getTLB_Slot(ipt_slot);
      if(tlb_slot != -1){
         IPTable[ipt_slot].dirty = machine->tlb[tlb_slot].dirty;
         machine->tlb[tlb_slot].valid = FALSE;
      }
      if(IPTable[ipt_slot].dirty){
         IPTSwapToFile(ipt_slot,vpn);
      }
   }
   IPTSwapToMemory(ipt_slot,vpn);
   updateIPTEntry(ipt_slot,vpn);			
   return ipt_slot;
}
void updateIPTEntry(int ipt_slot,int vpn)
{
#ifdef IPT_DEBUG_UPDATE
   printf("[updateIPTEntry]Start ipt_slot %d vpn %d\n",ipt_slot,vpn);
#endif
   //	VmTranslationEntry* pt = currentThread->space->getPageTable();
   //	int ppn = pt[ipt_slot].physicalPage;
   //IPTable[ipt_slot].pid = getPhysAddr(vpn);
   IPTable[ipt_slot].pid = currentThread->space->getProcessID();
   IPTable[ipt_slot].vpn= vpn; 
   IPTable[ipt_slot].space = currentThread->space;
   IPTable[ipt_slot].use = TRUE;
   IPTable[ipt_slot].valid = TRUE;
   IPTable[ipt_slot].readOnly = FALSE;
#ifdef IPT_DEBUG_UPDATE
   printf("[updateIPTEntry] ipt_slot %d \n",ipt_slot);
   printf("[updateIPTEntry] pid %d vpn %d \n",IPTable[ipt_slot].pid,
         IPTable[ipt_slot].vpn);
#endif
}
int getIPT_Slot()
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
int getFreeIPTSlot()
{
   //Try to match the same phys addr in ipt first
   //if(IPTable[ppn].valid == FALSE)
   //   return ppn;
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
void IPTSwapToFile(int ipt_slot,int vpn)
{
#ifdef IPT_DEBUG_SWAP_OUT
   printf("[IPTSwapToFile]Start ipt_slot %d\n",ipt_slot);
   printf("[IPTSwapToFile]vpn %d IPTable[%d].vpn %d\n",
         vpn,ipt_slot,IPTable[ipt_slot].vpn);
#endif
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
#ifdef IPT_DEBUG_SWAP_IN
   //   printf("[IPTSwapToMemory] Start! ipt_slot %d vpn %d\n",ipt_slot,vpn);
#endif
   VmTranslationEntry* pt = currentThread->space->getPageTable();
   switch(pt[vpn].location){
      case EXEC:
         IPTable[ipt_slot].vpn= vpn;
#ifdef IPT_DEBUG_SWAP_IN
         printf("[IPTSwapToMemory] Bring in Page[vpn %d] from execfile,IPTable[%d]\n" ,
               vpn,ipt_slot,IPTable[ipt_slot].vpn);
#endif
         currentThread->space->readExec(ipt_slot,vpn);
         pt[vpn].physicalPage = ipt_slot;//vpn;
         //pt[vpn].physicalPage;
         IPTable[ipt_slot].dirty = FALSE;
         break;
      case SWAP:
#ifdef IPT_DEBUG_SWAP_IN
         printf("[IPTSwapToMemory] Page[vpn %d] is in SWAP, ipt_slot %d\n" ,vpn,ipt_slot);
#endif
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
               {

#ifdef TLB_DEBUG
                  printf("[getTLB_Slot] Found TLB %d matach IPT %d\n",
                        i,ipt_slot);
#endif
                  return i;
               }   
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

#ifdef TLB_DEBUG
   printf("[TLB_Update]Start Using IPT ,ppn %d\n",ppn);
#endif
   bool dirty;
   int tlb_slot = getTLBSlot(&dirty);
   if(dirty)
      propagateToIPT(tlb_slot);
   updateTLBEntry(tlb_slot,ppn);	
#ifdef TLB_DEBUG
   printf("[TLB_Update]Using IPT ,ppn %d,tlb_slot %d\n",ppn,tlb_slot);
#endif
}
#endif
//---------------- TLB Update no IPT----------------------
void TLB_Update(int ppn,int vpn)
{
#ifdef TLB_DEBUG
   printf("[TLB_Update]Start No IPT,ppn %d,vpn %d\n",ppn,vpn);
#endif
   bool dirty;
   int tlb_slot = getTLBSlot(&dirty);
   updateTLBEntry(tlb_slot,ppn,vpn);	
#ifdef TLB_DEBUG
   printf("[TLB_Update]No IPT ,ppn %d,vpn %d,tlb_slot %d\n",ppn,vpn,tlb_slot);
#endif
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
#ifdef TLB_DEBUG
   printf("[updateTLBEntry]Start No IPT,slot %d,ppn %d,vpn %d\n",tlb_slot,ppn,vpn);
#endif
   machine->tlb[tlb_slot].virtualPage = vpn;
   machine->tlb[tlb_slot].physicalPage = ppn;
   machine->tlb[tlb_slot].use = TRUE;
   machine->tlb[tlb_slot].valid = TRUE;
   machine->tlb[tlb_slot].dirty = FALSE;
   machine->tlb[tlb_slot].readOnly;
#ifdef TLB_DEBUG
   printf("[updateTLBEntry]tlb[%d] vp %d,pp %d,use %s,valid %s,dirty %s\n",
         tlb_slot,
         machine->tlb[tlb_slot].virtualPage, 
         machine->tlb[tlb_slot].physicalPage,
         machine->tlb[tlb_slot].use ? "True" :"False", 
         machine->tlb[tlb_slot].valid? "True" :"False",   
         machine->tlb[tlb_slot].dirty? "True" :"False"  
         );
#endif
}
#ifdef HAVE_IPT
void updateTLBEntry(int tlb_slot,int ppn)
{
#ifdef TLB_DEBUG
   printf("[updateTLBEntry]With IPT,slot %d,ppn %d,vpn %d\n",tlb_slot,ppn,IPTable[ppn].vpn);
#endif
   machine->tlb[tlb_slot].virtualPage = IPTable[ppn].vpn;
   machine->tlb[tlb_slot].physicalPage = ppn;
   machine->tlb[tlb_slot].use = TRUE;
   machine->tlb[tlb_slot].valid = TRUE;
   machine->tlb[tlb_slot].dirty = IPTable[ppn].dirty;
   machine->tlb[tlb_slot].readOnly;
#ifdef TLB_DEBUG
   printf("[updateTLBEntry]tlb[%d] vp %d,pp %d,use %s,valid %s,dirty %s\n",
         tlb_slot,
         machine->tlb[tlb_slot].virtualPage, 
         machine->tlb[tlb_slot].physicalPage,
         machine->tlb[tlb_slot].use ? "True" :"False", 
         machine->tlb[tlb_slot].valid? "True" :"False",   
         machine->tlb[tlb_slot].dirty? "True" :"False"  
         );
#endif
}
void propagateToIPT(int tlb_slot)
{
   int pp = machine->tlb[tlb_slot].physicalPage;
   int vp = machine->tlb[tlb_slot].virtualPage;
   //int cpid = getPhysAddr(vp);//currentThread->space->getProcessID();
   int cpid = currentThread->space->getProcessID();
   //printf("[ppgToIPT] cpid %d ,IPTable[pp %d].pid = %d\n",cpid,pp,IPTable[pp].pid);
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
//---------------- Project 4---------------------------
//-----------------------------------------------------


#ifdef PROJ4 
//----------------GetMailBox_Syscall-------------------
//Find the available mail box on the server
//Pre:None
//Post:None
//Return: the mail box id
int GetMailBox_Syscall()
{
   int port = -1;
   mailBoxLock->Acquire();
   port = mailBoxMap->Find();
   mailBoxLock->Release();
   return port;

}

//----------------GetMachineID_Syscall()----------------
//Get machine id when start the program passing form
//command line after -m "machine id"
//Pre:None
//Post:None
//Return: the machine id
int GetMachineID_Syscall()
{
   return machineID;
}
int combineIPPort(int ip,int port);
int getIP(int ipport);
int getPort(int ipport);

//-------------------ClerkReg_Syscall(int id)-----------
//The Clerk will call this function to register the server
//When Clerk is free, if there is the customer is waiting
//Server will return the massage with customer ID
//Otherwise, it will return "RCW" for to let clerk wait.
//
//Clerk Have four type 0:App,1:Pic,2:Pass,3:Cash
//
//Pre: Clerk IP,Clerk Port,Clerk Type
//Post:Customer IP,Customer Port  
//Return: None 
int ClerkReg_Syscall(int ip,int port,int type)
//int *customerIP,int *customerPort)
{
   printf("==============ClerkReg_Syscall=============\n");
   printf("ip %d port %d type %d\n",ip,port,type);
   int customerIP,customerPort;
   char typeName[20];
   char clerk_out_msg[20];
   char clerk_in_msg[20];
   char clerk_buf[256];
   PacketHeader Clerk_outPktHdr, Clerk_inPktHdr;
   MailHeader Clerk_outMailHdr, Clerk_inMailHdr;
   clerkRegLock->Acquire();
   ClearArray(clerk_out_msg);
   //Register App Clerk
   switch(type)
   {
      case 0:
         sprintf(clerk_out_msg, "RA %d %d \0", ip,port);//Reg App Clerk
         strcpy(typeName,"Application");
         break;
      case 1:
         sprintf(clerk_out_msg, "RP %d %d \0", ip,port);//Reg Pic Clerk
         strcpy(typeName,"Picture");
         break;
      case 2:
         sprintf(clerk_out_msg, "RS %d %d \0", ip,port);//Reg paSs Clerk
         strcpy(typeName,"Passport");
         break;
      case 3:
         sprintf(clerk_out_msg, "RH %d %d \0", ip,port);//Reg casH Clerk
         strcpy(typeName,"Cashier");
         break;
   }
   SR_server(port,clerk_out_msg,clerk_in_msg);
   clerkRegLock->Release();
   //if there is no customer can handle
   if(strcmp(clerk_in_msg, "RCW") == 0)
   {
      while(true)
      {
         printf("[Clerk]%s Clerk %d:%d wait for customer\n",typeName,ip,port);
         postOffice->Receive(port, &Clerk_inPktHdr, &Clerk_inMailHdr, clerk_buf);
         strcpy(clerk_in_msg, clerk_buf);
         printf("[Clerk]Client got \"%s\" from %d, box %d\n", 
            clerk_in_msg, Clerk_inPktHdr.from, Clerk_inMailHdr.from);
         fflush(stdout);
         if(strncmp(clerk_in_msg, "RCS",3) == 0)
         {
            
            sscanf(clerk_in_msg, "%*s %d %d", &customerIP,&customerPort);
            printf("[Clerk]%s Clerk has get the Customer %d:%d! \n",
               typeName,customerIP,customerPort);
            return combineIPPort(customerIP,customerPort);
         }
      }

   }
   sscanf(clerk_in_msg, "%*s %d %d", &customerIP,&customerPort);
   printf("[Clerk]%s Clerk has get the Customer %d:%d! \n",
         typeName,customerIP,customerPort);
   return combineIPPort(customerIP,customerPort);

}
//-----------------CustomerAcquire_Syscall(int id)---------
//Customer call this function to get clerk to service
//It will do the same thing as the clerk did
//Customer send the clerk request to the server
//If there is avaiable clerk on the server,
//Server will return the ACS message with clerk ID
//Otherwise, it will queue up the customer request 
//until server get clerk registion
//
//Pre: customer ID,Clerk Type
//Post: None
//Return: Clerk ID
int CustomerAcquire_Syscall(int ip,int port,int type)//,int *clerkIP,int *clerkPort)
{
   char typeName[20];
   int clerkIP,clerkPort;
   char customer_out_msg[20];
   char customer_in_msg[20];
   char customer_buf[256];
   PacketHeader Customer_outPktHdr, Customer_inPktHdr;
   MailHeader Customer_outMailHdr, Customer_inMailHdr;
   customerAcquireLock->Acquire();
   ClearArray(customer_out_msg);
   //Acquire App Clerk
   switch(type)
   {
      case 0:
         sprintf(customer_out_msg, "AAC %d %d\0", ip,port);//Acquire App Clerk
         strcpy(typeName,"Application");
         break;
      case 1:
         sprintf(customer_out_msg, "APC %d %d\0", ip,port);//Acquire Pic Clerk
         strcpy(typeName,"Picture");
         break;
      case 2:
         sprintf(customer_out_msg, "ASC %d %d\0", ip,port);//Acquire paSs Clerk
         strcpy(typeName,"Passport");
         break;
      case 3:
         sprintf(customer_out_msg, "AHC %d %d\0", ip,port);//Acquire casH Clerk
         strcpy(typeName,"Cashier");
         break;
   }
   SR_server(port,customer_out_msg,customer_in_msg);
   customerAcquireLock->Release();
   //if there is no clerk can serve customer 
   if(strcmp(customer_in_msg, "ACW") == 0)
   {
      while(true)
      {
         printf("[Customer]Customer %d:%d wait for %s clerk\n",ip,port,typeName);
         postOffice->Receive(port, &Customer_inPktHdr, &Customer_inMailHdr, customer_buf);
         strcpy(customer_in_msg, customer_buf);
         printf("[Customer]Client got \"%s\" from %d, box %d\n", customer_in_msg, Customer_inPktHdr.from, Customer_inMailHdr.from);
         fflush(stdout);
         if(strncmp(customer_in_msg, "ACS",3) == 0)
         {
            
            sscanf(customer_in_msg, "%*s %d %d", &clerkIP,&clerkPort);
            printf("[Customer]Customer has get the %s Clerk %d:%d! \n",typeName,clerkIP,clerkPort);
            return combineIPPort(clerkIP,clerkPort);
         }
      }

   }
   sscanf(customer_in_msg, "%*s %d %d", &clerkIP,&clerkPort);
   printf("[Customer]Customer has get the %s Clerk %d:%d! \n",typeName,clerkIP,clerkPort);
   return combineIPPort(clerkIP,clerkPort);

}
#endif
//-----------------------------------------------------
//---------------- EXCEPTION HANDLER ------------------
//-----------------------------------------------------

void ExceptionHandler(ExceptionType which) {

   int type = machine->ReadRegister(2); // Which syscall?
   int rv=0; 	// the return value from a syscall

#ifdef NETWORK
   bool reading;
   int tempo;
   int vaddress;
   char name[15];
   int i;
#endif
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
#ifdef NETWORK
         case SC_CreateLock:
            DEBUG('a', "CreateLock syscall.\n");
//
//               name[0] = '\0';
//            i=0;
//            vaddress = machine->ReadRegister(4);
//            reading = machine->ReadMem(vaddress, 1, &tempo);
//            while(reading && tempo != 0)
//            {	
//               //strncat(name, (char*)tempo, 1);
//               name[i] = tempo;
//               vaddress++;
//               i++;
//               reading = machine->ReadMem(vaddress, 1, &tempo);
//            }
            rv =  CreateLock_Syscall(machine->ReadRegister(4));
            break;

         case SC_CreateCondition:
            DEBUG('a', "CreateCondition syscall.\n");
//               name[0] = '\0';
//            i=0;
//            vaddress = machine->ReadRegister(4);
//            reading = machine->ReadMem(vaddress, 1, &tempo);
//            while(reading && tempo != 0)
//            {	
//               //strncat(name, (char*)tempo, 1);
//               name[i] = tempo;
//               vaddress++;
//               i++;
//               reading = machine->ReadMem(vaddress, 1, &tempo);
//            }
            rv =  CreateCondition_Syscall(machine->ReadRegister(4));
            break;

#else
         case SC_CreateLock:
            DEBUG('a', "CreateLock syscall.\n");
            rv = CreateLock_Syscall();
            break;

         case SC_CreateCondition:
            DEBUG('a', "CreateCondition syscall.\n");
            rv = CreateCondition_Syscall();
            break;
#endif

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
            //printf("Exec syscall.\n");
            DEBUG('a', "Exec syscall.\n");
            rv =  Exec_Syscall(machine->ReadRegister(4));
            break;

         case SC_Exit:
            DEBUG('a', "Exit syscall.\n");
            Exit_Syscall(machine->ReadRegister(4));
            break;

         case SC_Yield:
            DEBUG('a', "Yield syscall.\n");
            Yield_Syscall();
            break;
         case SC_ClerkReg:
            DEBUG('a', "Clekr Reg syscall.\n");
            rv = ClerkReg_Syscall(machine->ReadRegister(4),machine->ReadRegister(5),
            machine->ReadRegister(6)
            );
            break;
         case SC_CustomerAcquire:
            DEBUG('a', "Clekr Reg syscall.\n");
            rv =
            CustomerAcquire_Syscall(machine->ReadRegister(4),machine->ReadRegister(5),
            machine->ReadRegister(6)
            );
            break;
         case SC_GetMachineID:
            DEBUG('a', "Get Machine ID syscall.\n");
            rv = GetMachineID_Syscall();
            break;
         case SC_GetMailBox:
            DEBUG('a', "Get Mail Box syscall.\n");
            rv = GetMailBox_Syscall();
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

