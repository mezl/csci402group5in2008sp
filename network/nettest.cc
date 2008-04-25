// nettest.cc 
//	Test out message delivery between two "Nachos" machines,
//	using the Post Office to coordinate delivery.
//
//	Two caveats:
//	  1. Two copies of Nachos must be running, with machine ID's 0 and 1:
//		./nachos -m 0 -o 1 &
//		./nachos -m 1 -o 0 &
//
//	  2. You need an implementation of condition variables,
//	     which is *not* provided as part of the baseline threads 
//	     implementation.  The Post Office won't work without
//	     a correct implementation of condition variables.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"
#include <string.h>
#include "../threads/synch.h"
#include "table.h"
#include <stdlib.h>
#include "list.h"

//If want to display the sending/receive massage
//define this line
//#define SR_DEBUG

// Test out message delivery, by doing the following:
//	1. send a message to the machine with ID "farAddr", at mail box #0
//	2. wait for the other machine's message to arrive (in our mailbox #0)
//	3. send an acknowledgment for the other machine's message
//	4. wait for an acknowledgement from the other machine to our 
//	    original message



   void
MailTest(int farAddr)
{
   PacketHeader outPktHdr, inPktHdr;
   MailHeader outMailHdr, inMailHdr;
   char *data = "Hello there!";
   char *ack = "Got it!";
   char buffer[MaxMailSize];

   // construct packet, mail header for original message
   // To: destination machine, mailbox 0
   // From: our machine, reply to: mailbox 1
   outPktHdr.to = farAddr;		
   outMailHdr.to = 0;
   outMailHdr.from = 1;
   outMailHdr.length = strlen(data) + 1;

   // Send the first message
   bool success = postOffice->Send(outPktHdr, outMailHdr, data); 

   if ( !success ) {
      printf("The postOffice Send failed. You must not have the other Nachos running. Terminating Nachos.\n");
      interrupt->Halt();
   }

   // Wait for the first message from the other machine
   postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
   printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
   fflush(stdout);

   // Send acknowledgement to the other machine (using "reply to" mailbox
   // in the message that just arrived
   outPktHdr.to = inPktHdr.from;
   outMailHdr.to = inMailHdr.from;
   outMailHdr.length = strlen(ack) + 1;
   success = postOffice->Send(outPktHdr, outMailHdr, ack); 

   if ( !success ) {
      printf("The postOffice Send failed. You must not have the other Nachos running. Terminating Nachos.\n");
      interrupt->Halt();
   }

   // Wait for the ack from the other machine to the first message we sent.
   postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
   printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
   fflush(stdout);

   // Then we're done!
   interrupt->Halt();
}



class NETWORK_LOCK
{
   public:
      int lockID;
      char name[15];
      int ownerID;
      List* waitQueue;

      NETWORK_LOCK()
      {
         lockID = -1;
         name[0] = '\0';
         ownerID = -1;
         waitQueue = new List();
      }
      NETWORK_LOCK(int creatorID, char* lockName)
      {
         strcpy(name, lockName);
         ownerID = creatorID;
         waitQueue = new List();
      }
      ~NETWORK_LOCK()
      {
         delete waitQueue;
      }
};

class NETWORK_COND
{
   public:
      int condID;
      char name[15];
      int lockID;
      List* waitQueue;

      NETWORK_COND()
      {
         condID = -1;
         name[0] = '\0';
         lockID = -1;
         waitQueue = new List();
      }
      NETWORK_COND(char* condName)
      {
         strcpy(name, condName);
         waitQueue = new List();
      }
      ~NETWORK_COND()
      {
         delete waitQueue;
      }
};


//Table serverLockTable(100);
//Table serverCondTable(100);
//map<char*, NETWORK_LOCK> lockMap;
//map<char*, NETWORK_COND> condMap;
//std::map <char*, std::vector<NETWORK_LOCK> > lockMap;
//std::map <char*, std::vector<NETWORK_COND> > condMap;
NETWORK_LOCK lockArray[100];
NETWORK_COND condArray[100];
int lockCount;
int condCount;



// Functions to support the Lock and Cond Array

void CleanMessage(char name[15])
{
   for(int i=0; i<15; i++)
      name[i] = '\0';
}


//send a wakeup message to process
void WakeProcessUp(int callerID)
{
   PacketHeader outPktHdr;
   MailHeader outMailHdr;

   outPktHdr.to = callerID;
   outMailHdr.to = 1;
   outMailHdr.from = 1;
   outMailHdr.length = strlen("UP")+1;

   printf("[SERVER]Sending a packet of \"%s\" to %d, box %d\n", "UP", outPktHdr.to, outMailHdr.to);
   postOffice->Send(outPktHdr, outMailHdr, "UP");
}

//send a lock acquired message to process
void PassLockToProcess(int callerID)
{
   PacketHeader outPktHdr;
   MailHeader outMailHdr;

   outPktHdr.to = callerID;
   outMailHdr.to = 1;
   outMailHdr.from = 1;
   outMailHdr.length = strlen("LAS")+1;

   printf("[SERVER]Sending a packet of \"%s\" to %d, box %d\n", "LAS", outPktHdr.to, outMailHdr.to);
   postOffice->Send(outPktHdr, outMailHdr, "LAS");
}

void ClearLockArray()
{
   int i;
   for(i=0; i<100; i++)
   {
      lockArray[i].lockID = -1;
      lockArray[i].name[0] = '\0';
      lockArray[i].ownerID = -1;
      //List* waitQueue = new List;
   }
}
void ClearConditionArray()
{
   int i;
   for(i=0; i<100; i++)
   {
      condArray[i].condID = -1;
      condArray[i].name[0] = '\0';
      condArray[i].lockID = -1;
      //List* waitQueue= new List;
   }
}
int FindLock(char* lockName)
{
   //return -1 if lockName is not found in array
   //return lock index if lockName is found in array

   for(int i=0; i<100; i++)
   {
      if(strcmp(lockArray[i].name, lockName) == 0)
         return i;
   }

   //no lock found
   return -1;
}

int FindCondition(char* condName)
{
   //return -1 if condName is not found in array
   //return condition index if condName is found in array

   for(int i=0; i<100; i++)
   {
      if(strcmp(condArray[i].name, condName) == 0)
         return i;
   }

   //no condition found
   return -1;
}

int CreateLock(char* lockName)
{
   //return lock index on successful add, return 0 if failed

   //Disable interrupts
   IntStatus oldLevel = interrupt->SetLevel(IntOff);
   int i = 0;
   int success = 0;
   int temp;

   //find if lock has already exist in the array
   temp = FindLock(lockName);
   if(temp != -1)
   {
      (void) interrupt->SetLevel(oldLevel);
      return temp;
   }

   //lock haven't exist in array, find an empty spot to create new lock
   while(success == 0)
   {	
      //find empty slot in lock array
      if(lockArray[i].name[0] == '\0')
      {
         lockArray[i].lockID = i;
         strcpy(lockArray[i].name, lockName);
         lockArray[i].ownerID = -1;
         lockArray[i].waitQueue = new List;
         lockCount++;
         success = 1;
         (void) interrupt->SetLevel(oldLevel);
         return i;
      }
      i++;
   }
   (void) interrupt->SetLevel(oldLevel);
   return success;
}

int DestroyLock(int lockID, int processID)
{
   //return 1 on successful destroy, return 0 if failed
   if(lockArray[lockID].ownerID == processID || lockArray[lockID].ownerID == -1)
   {
      lockArray[lockID].lockID = -1;
      lockArray[lockID].name[0] = '\0';
      lockArray[lockID].ownerID = -1;
      //delete lockArray[lockID].waitQueue;
      lockCount--;
      return 1;
   }
   else
   {
      printf("[SERVER]Lock %d is still being used by someone else, Destroy failed", lockID);
   }
   return 0;
}

int Acquire(int lockID, int processID)
{
   //return 0 if fail, 1 if success, 2 if need wait 
   int success = 0;
   if(lockArray[lockID].name[0] == '0') 
   {
      printf("[SERVER] Lock %d doesn't exist anymore, can't be acquired\n", lockID);
      success = 0;
   }
   else
   {
      if(lockArray[lockID].ownerID == -1)
      {
         printf("[SERVER] Machine %d become Lock %d owner. Acquire success\n", processID, lockID);	
         lockArray[lockID].ownerID = processID;
         success = 1;
      }
      else if(lockArray[lockID].ownerID == processID)
      {
         printf("[SERVER] Machine %d is already Lock %d owner. Acquire aborted\n", processID, lockID);	
         success = 1;
      }
      else
      {
         printf("[SERVER] Lock %d belong to someone else. Acquire wait\n", lockID, processID);	
         lockArray[lockID].waitQueue->Append((void*)processID);
         success = 2;
      }
   }

   return success;
}

int Release(int lockID, int processID)
{
   //return 0 if fail, 1 if success
   int success = 0;
   int callerID= -1;
   if(lockArray[lockID].name[0] == '\0')
   {
      printf("[SERVER] Lock %d doesn't exist anymore. Release failed\n", lockID);
      success = 0;
   }
   else if(lockArray[lockID].ownerID == processID)
   {
      printf("[SERVER] Machine %d is releasing Lock %d. Release success\n", processID, lockID);	
      lockArray[lockID].ownerID = -1;
      success = 1;
      if(!lockArray[lockID].waitQueue->IsEmpty())
      {
         callerID = (int)(lockArray[lockID].waitQueue->Remove());
         lockArray[lockID].ownerID = callerID;
         PassLockToProcess(callerID);
      }
   }
   else if(lockArray[lockID].ownerID == -1)
   {
      printf("[SERVER] Lock %d already released. Release success\n", lockID);	
      success = 1;
   }	
   else
   {
      printf("[SERVER] Machine %d is  not Lock %d owner. Release failed\n", processID, lockID);
      success = 0;
   }
   return success;
}

int CreateCondition(char* condName)
{
   //return condition index on successful add, return 0 if failed

   //Disable interrupts
   IntStatus oldLevel = interrupt->SetLevel(IntOff);
   int i = 0;
   int success = 0;
   int temp;

   //find if condition has already exist in the array
   temp = FindCondition(condName);
   if(temp != -1)
   {
      (void) interrupt->SetLevel(oldLevel);
      return temp;
   }

   //cond haven't exist in array, find an empty spot to create new cond
   while(success == 0)
   {	
      //find empty slot in condition array
      if(condArray[i].name[0] == '\0')
      {
         condArray[i].condID = i;
         strcpy(condArray[i].name, condName);
         condArray[i].waitQueue = new List;
         condCount++;
         success = 1;
         (void) interrupt->SetLevel(oldLevel);
         return i;
      }
      i++;
   }
   (void) interrupt->SetLevel(oldLevel);
   return success;
}

int DestroyCondition(int ID, int processID)
{
   //return 1 on successful destroy, return 0 if failed
   condArray[ID].lockID = -1;
   condArray[ID].condID = -1;
   condArray[ID].name[0] = '\0';
   //delete condArray[ID].waitQueue;
   condCount--;
   return 1;
}

int Signal(int lockID, int condID, int processorID)
{
   //return 1 on successful Signal, return 0 if failed
   int callerID;

   if(lockArray[lockID].ownerID != processorID)
   {
      printf("[SERVER]Process %d\n is not lock %d owner, Signal failed", processorID, lockID);
      return 0;
   }
   printf("[SERVER]Process %d is signaling condition %d \n", processorID, condID);
   if(!condArray[condID].waitQueue->IsEmpty())
   {
      callerID = (int)(condArray[condID].waitQueue->Remove());
      WakeProcessUp(callerID);
   }

   return 1;
}

int Wait(int lockID, int condID, int processorID)
{
   //return 1 on successful Signal, return 0 if failed

   if(lockArray[lockID].ownerID != processorID)
   {
      printf("[SERVER]Process %d\n is not lock %d owner, Wait failed", processorID, lockID);
      return 0;
   }
   lockArray[lockID].ownerID = -1;
   condArray[condID].waitQueue->Append((void*)processorID);	

   return 1;
}

int Broadcast(int lockID, int condID, int processorID)
{
   //return 1 on successful Signal, return 0 if failed

   while(condArray[condID].waitQueue->IsEmpty() != true)
      Signal(lockID, condID, processorID);

   return 0;
}

void Initialization()
{
   lockCount = 0;
   condCount = 0;
   ClearLockArray();
   ClearConditionArray();
}


// server test added
// this is the server kernal+stub

char gckname[20];
//------getClerkName(int type)-----------------------
//Convert the clerk type to the string name
//Pre:Clerk type (int)
//Post:
//Return:Clerk name(char *)
char* getClerkName(int type)
{
   if(type == 0)
      sprintf(gckname,"Application");
   else if(type == 1)   
      sprintf(gckname,"Picture");
   else if(type == 2)   
      sprintf(gckname,"Passport");
   else if(type == 3)   
      sprintf(gckname,"Cashier");

  return gckname;    

}

//---------combineIPPort(int ip,int port)-----------
//Combine ip & port to one number for list to use
//for the simple use in the table
//We combine the ip and port to one number
//so the 10^1's number is ip,10^0 is port
//Example:15 => ip = 1,port = 5
//We assume the ip & port are less then 10
//Pre:ip,port
//Post:None
//Return:one number combine with
int combineIPPort(int ip,int port)
{
   return ip*10+port;
}
int getIP(int ipport)
{
   return ipport/10;
}
int getPort(int ipport)
{
   return ipport%10;
}
//-----------Send & Receive Function-------------------
//Send the message and get message for other server machine
//Default port for sending is 1 and listen port 1 for reply
//This is the communication between server and server
//The server<-->client will use port 0

////send a message to other machine 
//void sendTo(int ip,int port,char *msg,int myport)
//{
//   printf("[SendTo] ip %d port %d msg \"%s\" \n", ip,port,msg);
//	PacketHeader outPktHdr;
//	MailHeader outMailHdr;
//
//	outPktHdr.to = ip;
//	outMailHdr.to = port;
//	outMailHdr.from = myport;
//	outMailHdr.length = strlen(msg)+1;
//	
//	printf("[SERVER]Sending a packet of \"%s\" to %d, box %d\n", msg, outPktHdr.to, outMailHdr.to);
//	postOffice->Send(outPktHdr, outMailHdr,msg); 
//}
void sendTo(int ip,int port,char * msg,int myport)
{
   PacketHeader s_outPktHdr ;
   MailHeader s_outMailHdr ;
   int result;
   s_outPktHdr.to = ip;
   s_outMailHdr.to = port;
   s_outMailHdr.from = myport;
   s_outMailHdr.length = strlen(msg)+1;
#ifdef SR_DEBUG
   printf("[SERVER%d]Sending a packet of \"%s\" to %d, box %d\n", machineID,msg, s_outPktHdr.to, s_outMailHdr.to);
#endif
   result= postOffice->Send(s_outPktHdr, s_outMailHdr, msg);

   if(!result)
   {
      printf("Server send reply message failed. Client probably not found. Terminating nachos.\n");
      interrupt->Halt();
   }
}
void sendToServer(int ip,char * msg)
{
   //the communication between server's
   //is using port 1 to distinguish wiht client request
   //we have fork another server for listen port 1
   //we send to other server port 1 for asking result
   //and we use the port 2 for the reply port
   //so, after we send the server request message
   //we go to listen port 2 for the replay
   sendTo(ip,1,msg,2);
}
void forwardToServer(int ip,char * msg)
{
   //Since it's the forward message, we need to send 
   //to other server's port 0
   //the sender's port is not important
   //since the forwardhandler will replace those infor
   //before passing to the client handler

   sendTo(ip,0,msg,0);
}
void receiveFrom(char *msg,int port)
{
   PacketHeader r_inPktHdr ;
   MailHeader r_inMailHdr ;
   postOffice->Receive(port, &r_inPktHdr, &r_inMailHdr, msg);
}
void serverListen(int port,PacketHeader* r_inPktHdr,MailHeader* r_inMailHdr,char* msg)
{
   postOffice->Receive(port, r_inPktHdr, r_inMailHdr, msg);

#ifdef SR_DEBUG
   printf("[SERVER%d]Port %d just received \"%s\" from machine %d, box %d \n", 
         machineID,port,msg, r_inPktHdr->from, r_inMailHdr->from);
#endif
   fflush(stdout);
}
void receiveFromServer(char *msg)
{
   //default server<->server port is 1
   receiveFrom(msg,2);
}
//Since server will reply to the message from the the source
//We need wap the new package to let other under this is a 
//forward package
//If the org msg is "ACK 1 0" from machine 2:3, then the format is 
//"F 2 3 ACK 1 0" 
void forwardMsg(int ip,char *msg,int sourceIP,int sourcePort)
{
   char tmp[256];
   sprintf(tmp,"F %d %d %s",sourceIP,sourcePort,msg);
   forwardToServer(ip,tmp);

}
//------------askServerForClerk(int type)--------------------
//Ask all other server does the clerk avaiable
//it will send the message to other server for the clerk
//if some server respones ACK message back, then we return the 
//server number back ,otherwise, we return -1 for not found
//Pre:Need Clerk Type
//Post:Asking all other server
//Return:ServerID,-1 for not found
int askServerForClerk(int type){
   //Q[C/K][A/P/S/H]
   char out_msg[20];
   char in_msg[20];

   if(type == 0)
      sprintf(out_msg,"QKA");
   else if(type == 1)   
      sprintf(out_msg,"QKP");
   else if(type == 2)   
      sprintf(out_msg,"QKS");
   else if(type == 3)   
      sprintf(out_msg,"QKH");
      
   printf("[SERVER%d]====Asking other server for %s Clerk =====\n",
   machineID,getClerkName(type));
   for(int i = 0;i < SERVER_NUM ;i++)
   {
      if(i != machineID){//Don't ask myself
         sendToServer(i,out_msg);
         //printf("[SERVER%d]====now waiting for server %d reply=====\n",machineID,i);
         receiveFromServer(in_msg); 
         //if there is the server have clerk we need
         if(strcmp(in_msg,"ACK") == 0){
            printf("[SERVER%d]server %d has %s Clerk \n",
                  machineID,i,getClerkName(type));
            return i;
         }   

      }

   }
   printf("[SERVER%d]All server don't have %s clerk ,queue up customer!\n",
   machineID,getClerkName(type));
   return -1;

}
//------------askServerForCustomer(int type)--------------------
//Ask all other server does the some customer is waiting 
//it will send the message to other server for the customer 
//if some server respones ACK message back, then we return the 
//server number back ,otherwise, we return -1 for not found
//Pre:Need Clerk Type
//Post:Asking all other server
//Return:ServerID,-1 for not found
int askServerForCustomer(int type){
   //Q[C/K][A/P/S/H]
   char out_msg[20];
   char in_msg[20];

   if(type == 0)
      sprintf(out_msg,"QCA");
   else if(type == 1)   
      sprintf(out_msg,"QCP");
   else if(type == 2)   
      sprintf(out_msg,"QCS");
   else if(type == 3)   
      sprintf(out_msg,"QCH");
   printf("[SERVER%d]====Asking other server for customer who needs %s Clerk =====\n",
   machineID,getClerkName(type));
   for(int i = 0;i < SERVER_NUM ;i++)
   {
      if(i != machineID){//Don't ask myself
         sendToServer(i,out_msg);
         //printf("[Server]====now waiting for %d server reply=====\n",i);
         receiveFromServer(in_msg); 
         //if there is the server have clerk we need
         if(strcmp(in_msg,"ACK") == 0){
            printf("[SERVER%d]server %d has customer who needs %s Clerk =====\n",
                  machineID,i,getClerkName(type));
            return i;
         }

      }

   }
   printf("[SERVER%d]No one has customer who needs %s clerk ,queue up clerk!\n",
   machineID,getClerkName(type));
   return -1;

}
int handleForwardRequest(char *buffer,char *replyMessage,
      PacketHeader* outPktHdr,
      MailHeader* outMailHdr)
{
   int fw = 0;
   if(buffer[0] == 'F'){
      fw = 1;
      int sourceIP,sourcePort;
      int destIP,destPort;
      int orgMsg[20];

      //"F 2 3 ACK 1 0" 
      printf("[SERVER%d]===Get a forward message [%s]===\n",machineID,buffer);
      sscanf(buffer, "%*s %d %d %s %d %d", &sourceIP,&sourcePort,orgMsg,&destIP,&destPort);

      //pretend the message is form client
      outPktHdr->from= sourceIP;
      outMailHdr->from = sourcePort;
      sprintf(buffer,"%s %d %d",orgMsg,destIP,destPort);
   }
   return fw;
}   
bool isClerkTableEmpty(int type)
{
   bool result ;
   switch(type)
   {
      case 0:
         result =  AppClerkTable->clerkTable->IsEmpty();
         break; 
      case 1:
         result =  PicClerkTable->clerkTable->IsEmpty();
         break; 
      case 2:
         result =  PassClerkTable->clerkTable->IsEmpty();
         break; 
      case 3:
          result =  CashClerkTable->clerkTable->IsEmpty();
         break;
   }
   return result;

}
int getClerkTableRemove(int type)
{
   int result ;
   switch(type)
   {
      case 0:
         result = (int)(AppClerkTable->clerkTable->Remove());
         break; 
      case 1:
         result = (int)(PicClerkTable->clerkTable->Remove());
         break; 
      case 2:
         result = (int)(PassClerkTable->clerkTable->Remove());
         break; 
      case 3:
         result = (int)(CashClerkTable->clerkTable->Remove());
         break;
   }
   return result;

}
void handleCustomerRequest(char *buffer,char* replyMessage,int* noReply,
      PacketHeader* inPktHdr,
      MailHeader* inMailHdr)
{

   if(buffer[0] == 'A') {
      //Handle the customer Acquire Clerk
      //Format A[A/P/S/H]C CustomerID
      //Example: AAC 10 : customer 10 need appclerk
      //Return Message
      //A[APSH]S ClerkID: pass the clerk to customer
      //A[APSH]W : need customer wait

      //1.Handle the customer acquire clerk request
      //2.Get customerIP
      int customerIP,customerPort;
      sscanf(buffer, "%*s %d %d", &customerIP,&customerPort);
      //3.Check which type of clerk request;
      switch(buffer[1])
      {
         case 'A':
            printf("====Server%d Get AA request, customer need app clerk====\n",
            machineID);
            //if there is no clerk is free 
            if(AppClerkTable->clerkTable->IsEmpty())
            {
               //If the server don't have the clerk to handle customer
               //Then the server ask the other server if they have clerk avaiable
               //If all the server don't have the type of clerk avaiable
               //The server will queue up this customer, for latter on other server
               //may asking for the customer

               //In case while we are asking other server,
               //the same the server also ask us for the clerk/customer
               //So we first put our customer in the queue before we ask
               //then if other server need this customer
               //we remove from our queue and forward to them
               
               AppLine->Append((void*)combineIPPort(customerIP,customerPort));
               //Ask Other Server for this type of clerk
               int forwardID = askServerForClerk(0);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of clerk
               {
                  (int)(AppLine->Remove());
                  
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of clerk
                  //AppLine->Append((void*)combineIPPort(customerIP,customerPort));
                  sprintf(replyMessage, "ACW");//Acquire AppClerk Wait 
                  printf("[SERVER%d]Customer %d:%d is waiting for App\n",
                       machineID, customerIP,customerPort);
               }
            }else
            {
               //if there is clerk avaible ,pass the clerk id
               //to the customer 
               int clerkIP,clerkPort,tmpIPort;            
               tmpIPort = (int)(AppClerkTable->clerkTable->Remove());
               clerkIP = getIP(tmpIPort);
               clerkPort = getPort(tmpIPort);
               sprintf(replyMessage, "ACS %d %d",clerkIP,clerkPort);
               //Register Clerk Success
               printf("[SERVER%d]Customer %d:%d get appClerk %d:%d \n",
                     machineID,customerIP,customerPort,clerkIP,clerkPort);
               //Also Send Message to Clerk to wake Clerk UP
               char tmpBuf[256];
               sprintf(tmpBuf, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               sendTo(clerkIP,clerkPort,tmpBuf,0);


            }
            break;
         case 'P':
            printf("====Server%d Get AP request, customer need pic clerk====\n",
            machineID);
            //if there is no clerk is free 
            if(PicClerkTable->clerkTable->IsEmpty())
            {
               PicLine->Append((void*)combineIPPort(customerIP,customerPort));
               int forwardID = askServerForClerk(1);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of clerk
               {
                  (int)(PicLine->Remove());
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of clerk
                  //PicLine->Append((void*)combineIPPort(customerIP,customerPort));
                  sprintf(replyMessage, "ACW");//Acquire PicClerk Wait 
                  printf("[SERVER%d]Customer %d:%d is waiting for Pic\n",
                        machineID,customerIP,customerPort);
               }
            }else
            {
               //if there is clerk avaible ,pass the clerk id
               //to the customer 
               int clerkIP,clerkPort,tmpIPort;            
               tmpIPort = (int)(PicClerkTable->clerkTable->Remove());
               clerkIP = getIP(tmpIPort);
               clerkPort = getPort(tmpIPort);
               sprintf(replyMessage, "ACS %d %d",clerkIP,clerkPort);
               //Register Clerk Success
               printf("[SERVER%d]Customer %d:%d get picClerk %d:%d \n",
                     machineID,customerIP,customerPort,clerkIP,clerkPort);
               //Also Send Message to Clerk to wake Clerk UP
               char tmpBuf[256];
               sprintf(tmpBuf, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               sendTo(clerkIP,clerkPort,tmpBuf,0);


            }
            break;
         case 'S':
            printf("====Server%d Get As request, customer need passport clerk====\n",
            machineID);
            //if there is no clerk is free 
            if(PassClerkTable->clerkTable->IsEmpty())
            {
               PassLine->Append((void*)combineIPPort(customerIP,customerPort));
               int forwardID = askServerForClerk(2);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of clerk
               {
                  (int)(PassLine->Remove());
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of clerk
                  //PassLine->Append((void*)combineIPPort(customerIP,customerPort));
                  sprintf(replyMessage, "ACW");//Acquire PassClerk Wait 
                  printf("[SERVER%d]Customer %d:%d is waiting for Pass\n",
                        machineID,customerIP,customerPort);
               }
            }else
            {
               //if there is clerk avaible ,pass the clerk id
               //to the customer 
               int clerkIP,clerkPort,tmpIPort;            
               tmpIPort = (int)(PassClerkTable->clerkTable->Remove());
               clerkIP = getIP(tmpIPort);
               clerkPort = getPort(tmpIPort);
               sprintf(replyMessage, "ACS %d %d",clerkIP,clerkPort);
               //Register Clerk Success
               printf("[SERVER%d]Customer %d:%d get passClerk %d:%d \n",
                     machineID,customerIP,customerPort,clerkIP,clerkPort);
               //Also Send Message to Clerk to wake Clerk UP
               char tmpBuf[256];
               sprintf(tmpBuf, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               sendTo(clerkIP,clerkPort,tmpBuf,0);
            }
            break;
         case 'H':
            printf("====Server%d Get AH request, customer need cashier clerk====\n",
            machineID);
            //if there is no clerk is free 
            if(CashClerkTable->clerkTable->IsEmpty())
            {
               CashLine->Append((void*)combineIPPort(customerIP,customerPort));
               int forwardID = askServerForClerk(3);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of clerk
               {
                  (int)(CashLine->Remove());
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of clerk
                  //CashLine->Append((void*)combineIPPort(customerIP,customerPort));
                  sprintf(replyMessage, "ACW");//Acquire CashClerk Wait 
                  printf("[SERVER%d]Customer %d:%d is waiting for Cash\n",
                        machineID,customerIP,customerPort);
               }
            }else
            {
               //if there is clerk avaible ,pass the clerk id
               //to the customer 
               int clerkIP,clerkPort,tmpIPort;            
               tmpIPort = (int)(CashClerkTable->clerkTable->Remove());
               clerkIP = getIP(tmpIPort);
               clerkPort = getPort(tmpIPort);
               sprintf(replyMessage, "ACS %d %d",clerkIP,clerkPort);
               //Register Clerk Success
               printf("[SERVER%d]Customer %d:%d get cashClerk %d:%d \n",
                    machineID, customerIP,customerPort,clerkIP,clerkPort);
               //Also Send Message to Clerk to wake Clerk UP
               char tmpBuf[256];
               sprintf(tmpBuf, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               sendTo(clerkIP,clerkPort,tmpBuf,0);


            }
            break;

         default:
            break;
      }
   }

}
void handleClerkRequest(char *buffer,char* replyMessage,int* noReply,
      PacketHeader* inPktHdr,
      MailHeader* inMailHdr)
{

   if(buffer[0] == 'R'){
      //The format is like "RAA 10" for Reg AppClerk Available
      //buffer[1] is 'A' = AppClerk                      
      //buffer[1] is 'P' = PicClerk                      
      //buffer[1] is 'S' = PassClerk                      
      //buffer[1] is 'H' = CashClerk                      
      //1.Handle the clerk register request
      //2.Get clerk IP & Port
      int clerkIP,clerkPort;
      sscanf(buffer, "%*s %d %d", &clerkIP,&clerkPort);
      //3.Check which type of clerk;
      switch(buffer[1])
      {
         case 'A':
            //if there is no customer is waiting
            if(AppLine->IsEmpty())
            {
               int forwardID = askServerForCustomer(0);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of customer 
               {
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of customer waiting 

                  AppClerkTable->clerkTable->Append((void*)combineIPPort(clerkIP,clerkPort));
                  sprintf(replyMessage, "RCW");//Register Clerk Wait 
                  printf("[SERVER%d]App Clerk %d is waiting\n", machineID,clerkIP);
               }
            }else
            {
               //if there is customer is waiting,pass the customer id
               //to the register clerk
               int tmpIPort,customerIP,customerPort;            
               tmpIPort = (int)(AppLine->Remove());

               customerIP = getIP(tmpIPort);
               customerPort = getPort(tmpIPort); 
               sprintf(replyMessage, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               printf("[SERVER%d]App Clerk %d:%d get customer %d:%d \n",
                     machineID,clerkIP,clerkPort,customerIP,customerPort);
               //Also Send Message to Customer to wake Customer UP
               char tmpBuf[256];
               sprintf(tmpBuf,"ACS %d %d ",clerkIP,clerkPort);
               //Acquire Clerk Success
               sendTo(customerIP,customerPort,tmpBuf,0);

            }
            break;
         case 'P':
            if(PicLine->IsEmpty())
            {

               int forwardID = askServerForCustomer(1);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of customer 
               {
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of customer waiting 
                  PicClerkTable->clerkTable->Append((void*)combineIPPort(clerkIP,clerkPort));
                  sprintf(replyMessage, "RCW");//Register Clerk Wait 
                  printf("[SERVER%d]Pic Clerk %d is waiting\n", machineID,clerkIP);
               }
            }else
            {
               //if there is customer is waiting,pass the customer id
               //to the register clerk
               int tmpIPort,customerIP,customerPort;            
               tmpIPort = (int)(PicLine->Remove());

               customerIP = getIP(tmpIPort);
               customerPort = getPort(tmpIPort); 
               sprintf(replyMessage, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               printf("[SERVER%d]Pic Clerk %d:%d get customer %d:%d \n",
                     machineID,clerkIP,clerkPort,customerIP,customerPort);
               //Also Send Message to Customer to wake Customer UP
               char tmpBuf[256];
               sprintf(tmpBuf,"ACS %d %d ",clerkIP,clerkPort);
               //Acquire Clerk Success
               sendTo(customerIP,customerPort,tmpBuf,0);

            }
            break;
         case 'S':
            if(PassLine->IsEmpty())
            {

               int forwardID = askServerForCustomer(2);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of customer 
               {
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of customer waiting 
                  PassClerkTable->clerkTable->Append((void*)combineIPPort(clerkIP,clerkPort));
                  sprintf(replyMessage, "RCW");//Register Clerk Wait 
                  printf("[SERVER%d]Pass Clerk %d is waiting\n", machineID,clerkIP);
               }
            }else
            {
               //if there is customer is waiting,pass the customer id
               //to the register clerk
               int tmpIPort,customerIP,customerPort;            
               tmpIPort = (int)(PassLine->Remove());

               customerIP = getIP(tmpIPort);
               customerPort = getPort(tmpIPort); 
               sprintf(replyMessage, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               printf("[SERVER%d]Pass Clerk %d:%d get customer %d:%d \n",
                     machineID,clerkIP,clerkPort,customerIP,customerPort);
               //Also Send Message to Customer to wake Customer UP
               char tmpBuf[256];
               sprintf(tmpBuf,"ACS %d %d ",clerkIP,clerkPort);
               //Acquire Clerk Success
               sendTo(customerIP,customerPort,tmpBuf,0);

            }
            break;
         case 'H':
            if(CashLine->IsEmpty())
            {

               int forwardID = askServerForCustomer(3);//send 0 for app clerk,return -1 mean's all server busy
               if(forwardID!=-1)//some server have this type of customer 
               {
                  forwardMsg(forwardID,buffer,inPktHdr->from,inMailHdr->from);
                  *noReply = 1;//after forward message,we don't need replay to client
               }else{ //if no server have this type of customer waiting 
                  CashClerkTable->clerkTable->Append((void*)combineIPPort(clerkIP,clerkPort));
                  sprintf(replyMessage, "RCW");//Register Clerk Wait 
                  printf("[SERVER%d]Cash Clerk %d is waiting\n", machineID,clerkIP);
               }
            }else
            {
               //if there is customer is waiting,pass the customer id
               //to the register clerk
               int tmpIPort,customerIP,customerPort;            
               tmpIPort = (int)(CashLine->Remove());

               customerIP = getIP(tmpIPort);
               customerPort = getPort(tmpIPort); 
               sprintf(replyMessage, "RCS %d %d",customerIP,customerPort);
               //Register Clerk Success
               printf("[SERVER%d]Cash Clerk %d:%d get customer %d:%d \n",
                     machineID,clerkIP,clerkPort,customerIP,customerPort);
               //Also Send Message to Customer to wake Customer UP
               char tmpBuf[256];
               sprintf(tmpBuf,"ACS %d %d ",clerkIP,clerkPort);
               //Acquire Clerk Success
               sendTo(customerIP,customerPort,tmpBuf,0);

            }
            break;

         default:
            break;
      }



   }
}
void handleLockRequest(char *buffer,char *replyMessage,
      PacketHeader* inPktHdr,
      MailHeader* inMailHdr)
{
   int lockID, processorID, result;
   char tempMessage[15];
   CleanMessage(tempMessage);
   if(buffer[0] == 'L') {
      /* Lock operation */
      switch(buffer[1])
      {
         case 'C':
            sscanf(buffer, "%*s %s", tempMessage);
            lockID = CreateLock(tempMessage);
            printf("[SERVER]Creating lock %d \n", lockID);
            sscanf(buffer, "%*s %s", tempMessage);
            if(lockID == -1)
               sprintf(replyMessage, "LCF");
            else
               sprintf(replyMessage, "LCS %d ", lockID);
            break;

         case 'D':
            sscanf(buffer, "%*s %d", &lockID);
            printf("[SERVER]Destroying lock %d \n", lockID);
            result = DestroyLock(lockID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "LDS");
            else
               sprintf(replyMessage, "LDF");
            break;

         case 'A':
            sscanf(buffer, "%*s %d", &lockID);
            printf("[SERVER]Acquiring lock %d \n", lockID);
            result = Acquire(lockID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "LAS");
            else if(result ==2)
               sprintf(replyMessage, "LAW");
            else 
               sprintf(replyMessage, "LAF");
            break;

         case 'R':
            sscanf(buffer, "%*s %d", &lockID);
            printf("[SERVER]Releasing lock %d \n", lockID);
            result = Release(lockID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "LRS");
            else
               sprintf(replyMessage, "LRF");
            break;

         default:break;
      }
   }
}
void handleConditionRequest(char *buffer,char* replyMessage,
      PacketHeader* inPktHdr,
      MailHeader* inMailHdr)
{

   int lockID, condID, processorID, result;
   char tempMessage[15];
   CleanMessage(tempMessage);
   char* delims = ".";
   if(buffer[0] == 'C')
   {
      /* Conditon operation */
      switch(buffer[1])
      {
         case 'C':
            sscanf(buffer, "%*s %s", tempMessage);
            condID = CreateCondition(tempMessage);
            printf("[SERVER]Creating condition %d \n", condID);
            sscanf(buffer, "%*s %s", tempMessage);
            if(condID == -1)
               sprintf(replyMessage, "CCF");
            else
               sprintf(replyMessage, "CCS %d ", condID);
            break;

         case 'D':
            sscanf(buffer, "%*s %d", &condID);
            printf("[SERVER]Destroying condition %d \n", condID);
            result = DestroyCondition(condID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "CDS");
            else
               sprintf(replyMessage, "CDF");
            break;

         case 'S':
            sscanf(buffer, "%*s %d %d", &lockID, &condID);
            printf("[SERVER]Signaling condition %d \n", condID);
            result = Signal(lockID, condID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "CSS");
            else
               sprintf(replyMessage, "CSF");
            break;

         case 'W':
            sscanf(buffer, "%*s %d %d", &lockID, &condID);
            printf("[SERVER]Waiting condition %d \n", condID);
            result = Wait(lockID, condID, inPktHdr->from);
            if(result == 1)
               sprintf(replyMessage, "CWS");
            else
               sprintf(replyMessage, "CWF");

            break;

         case 'B':
            /*strtok(buffer, delims);
              tempMessage = strtok(NULL, delims);
              lockID = atoi(tempMessage);
              tempMessage = strtok(NULL, delims);
              condID = atoi(tempMessage);
              myCond = (Condition*)serverCondTable.Get(condID);
              myLock = (Lock*)serverLockTable.Get(lockID);
              myCond->Broadcast(myLock);
              printf("[SERVER]Broadcasting condition %d with lock %d \n", condID, lockID);
              replyMessage = "CBS";*/
            break;

         default:break;
      }

   }
}
int handleServerRequest(char *buffer,char* replyMessage)
{
   //Handle The request from other server
   //The format is Q[C/K][A/P/S/H]
   //[C/K] = need Customer/clerK
   //[A/P/S/H] = clerk Type
   int reply = -1;
   if(buffer[0] == 'Q') {
      reply = 1;
      if(buffer[1] == 'K'){
         switch(buffer[2])
         {
            case 'A':
               if(AppClerkTable->clerkTable->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'P':
               if(PicClerkTable->clerkTable->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'S':
               if(PassClerkTable->clerkTable->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'H':
               if(CashClerkTable->clerkTable->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;

         }
      } else if(buffer[1] == 'C'){
         switch(buffer[2])
         {
            case 'A':
               if(AppLine->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'P':
               if(PicLine->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'S':
               if(PassLine->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
            case 'H':
               if(CashLine->IsEmpty())
                  sprintf(replyMessage, "NCK");//neg ack
               else
                  sprintf(replyMessage, "ACK");//ack
               break;
         }

      }//else if buffer[1]
            

   }
   return reply;
}

/* parse the message  buffer (message) is limited to max. size of 15 chars*/
// buffer[0] represent type of request(lock/condition)
// buffer[0] is 'L' = Lock operation request
// buffer[0] is 'C' = Condition operation request
// buffer[0] is 'R' = Register clerk operation request
// buffer[0] is 'A' = Acquire clerk operation request

// buffer[1] represent lock/condition related operations
// buffer[1] is 'C' = Create Lock/Condition
// buffer[1] is 'D' = Destroy Lock/Condition
// buffer[1] is 'A' = Acquire for lock
// buffer[1] is 'R' = Release for lock
// buffer[1] is 'S/W/B' = Signal/wait/broadcast for Condition

// buffer[2-14] are required char string or integer for the operation

void handleClientRequest()
{
   PacketHeader h_inPktHdr;
   MailHeader h_inMailHdr;
   char buffer[MaxMailSize];
   char replyMessage[MaxMailSize];
   bool sendSuccess;
   int noReply = 0;
   int isFw = 0;
   serverListen(0, &h_inPktHdr, &h_inMailHdr, buffer);
   
   isFw = handleForwardRequest(buffer,replyMessage, &h_inPktHdr, &h_inMailHdr);
   handleCustomerRequest (buffer,replyMessage,&noReply, &h_inPktHdr, &h_inMailHdr);
   handleClerkRequest(buffer,replyMessage,&noReply, &h_inPktHdr, &h_inMailHdr);
   //handleServerRequest(buffer,replyMessage);


   
   //If there is the forward message, we need find the soure to replay

   // Send the server reply message after complete parsing the outMessage
   // skip sending reply message if noReply=1
   if(noReply == 0)
   {
      /* create the reply package and send */
      int port = h_inMailHdr.from;
      int ip = h_inPktHdr.from;
      sendTo(ip,port,replyMessage,1);
   }
}
//------------void serverToServer()----------------------
//This is another thread using for reply the server request
//It will listen the port 1 for the message from other server
//So, the server will listen two port at same time
//port 0 is using for client(clerk/customer) request
//port 1 is using for other server quary
void serverToServer()
{
   printf("[SERVER%d]Server %d start s2s server... \n",machineID,machineID);
   while(1){
      PacketHeader h_inPktHdr;
      MailHeader h_inMailHdr;
      char buffer[MaxMailSize];
      char replyMessage[MaxMailSize];
      int reply = 0;
      //listen port 1 for the message comming from other server
      serverListen(1, &h_inPktHdr, &h_inMailHdr, buffer);
      //Check the request the generate
      reply = handleServerRequest(buffer,replyMessage);

      /* create the reply package and send */
      if(reply == 1){
         int port = h_inMailHdr.from;
         int ip = h_inPktHdr.from;
         //reply to sender by the port sender given
         //we use port 1 to send message
         //port should be equal 2
         sendTo(ip,port,replyMessage,1);
      }
   }

}

//----handleManagerRequest(char *buffer,char* replyMessage)-----
//The message form port 3 will be handle like this way
//
//RGMGR 7 : register form manager in location 7,
//          server will set managerID to 7 and also forward msg to other
//          server.
//FRMG 7: forward msg for manager reg 7,so we just set our managerID and don't
//          need for forward to the other server
//CASH 100 5 3 : get casiher in 5:3 for amount 100
//          it will send this amount to manager

//The message will be sent from manager server
//
//To Manager
//RMGS : register manager success
//GETCASH 100: get 100 
//
//To Other server
//FRMG 7: see above
//
//To Cashier
//GETCASH 100: once receive the cash 100 from cashier


int handleManagerRequest(char *buffer,char* replyMessage)
{
   //Handle The request from other server
   //The format is Q[C/K][A/P/S/H]
   //[C/K] = need Customer/clerK
   //[A/P/S/H] = clerk Type
   int reply = -1;
   int mgrID;
   if(buffer[0] == 'R') {
      sscanf(buffer, "%*s %d", &mgrID);
      managerID = mgrID;
      printf("[SERVER%d]Server get manager register in %d... \n",
            machineID,managerID);

      //Forward manager register to other server
      printf("[SERVER%d]Forward other server for manager in %d=====\n",
            machineID,managerID);

      char out_msg[20];
      sprintf(out_msg,"FRMG %d ",managerID);
      for(int i = 0;i < SERVER_NUM ;i++)
      {
         if(i != machineID){//Don't ask myself
            sendTo(i,3,out_msg,3);
         }   

      }
      sprintf(replyMessage,"RMGS");
      reply = 1;

   } else if(buffer[0] == 'F') {
      sscanf(buffer, "%*s %d", &mgrID);
      managerID = mgrID;
      printf("[SERVER%d]Get Forward manager in %d=====\n",
            machineID,managerID);
   } else if(buffer[0] == 'C') {
      int ip,port,amount;
      sscanf(buffer, "%*s %d %d %d", &amount,&ip,&port);
      printf("[SERVER%d]Get money %d form casier %d:%d\n",
            machineID,amount,ip,port);

            if(managerID != -1 && managerID > SERVER_NUM){
               char out_msg[20];
               sprintf(out_msg,"GETCASH %d ",amount);
               sendTo(managerID,3,out_msg,3);

               
            }else{
               printf("[SERVER%d]No manager can handle this money\n", machineID);
            }
               sprintf(replyMessage,"GETCASH %d ",amount);
               reply = 1;
            
      
      
   }
   return reply;
}
//------------void managerServer()----------------------
//This the communcation between server and manager
//It will listen the port 3 for the operation
//First operation is manager register
//Once the server get reqister from manager
//The server will forward this registion to other server
//
//Second operation is for cashier clerk to use
//Once get money from customer
//It will send the amount to server
//
//Once server get the money from cashier,
//Server will tell manager the money cashier get

//The managerID is the global variable in the system.h
void managerServer()
{
   while(1){
      PacketHeader h_inPktHdr;
      MailHeader h_inMailHdr;
      char buffer[MaxMailSize];
      char replyMessage[MaxMailSize];
      int reply = 0;
      //listen port 1 for the message comming from other server
      serverListen(3, &h_inPktHdr, &h_inMailHdr, buffer);
      //Check the request the generate
      reply = handleManagerRequest(buffer,replyMessage);

      /* create the reply package and send */
      if(reply == 1){
         int port = h_inMailHdr.from;
         int ip = h_inPktHdr.from;
         //reply to sender by the port sender given
         //we use port 1 to send message
         //port should be equal 2
         sendTo(ip,port,replyMessage,3);
      }
   }

}
void ServerTest()
{

   Initialization();

   Thread *t = new Thread("s2s");
   t->Fork((VoidFunctionPtr)serverToServer,0);
   Thread *m = new Thread("managerServer");
   m->Fork((VoidFunctionPtr)managerServer,0);
   printf("[SERVER%d]Server %d starting up... \n",machineID,machineID);
   while(true)
   {
      handleClientRequest();

   }
   interrupt->Halt();
}
