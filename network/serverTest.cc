//serverTest.cc
//this includes both the server App and server stub together

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "../threads/synch.h"

void serverTest()
{
	Table serverLockTable(100);
	Table serverCondTable(100);
	PacketHeader outPktHdr, inPktHdr;
	MailHeader outMailHdr, inMailHdr;
	char *replyMessage;
	char buffer[MaxMailSize];
	bool success;
	int lockID, condID;
	Lock *myLock;
	Condition *myCond;
	char* tempMessage;
	char* delims = " ";

	while(true)
	{
		postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
		printf("Server just received \"%s\" from machine %d, box %d \n", buffer, inPktHdr.from, inMailHdr.from);
		fflush(stdout);
		
		tempMessage = "    ";

		/* parse the message */
		if(buffer[0] == 'L')
		{
			/* Lock operation */
			switch(buffer[1])
			{
				case 'C':
				myLock = new Lock("New Lock");
				lockID = serverLockTable.Put(myLock);
				printf("Server Creating lock %d \n", lockID);
				sprintf(tempMessage, "%d", lockID);
				replyMessage = tempMessage;
					break;
				case 'D':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				serverLockTable.Remove(lockID);
				printf("Server Destroying lock %d \n", lockID);
				replyMessage = "LDS";
					break;
				case 'A':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				myLock = (Lock*)serverLockTable.Get(lockID);
				myLock->Acquire();
				printf("Server Acquiring lock %d \n", lockID);
				replyMessage = "LAS";
					break;
				case 'R':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				myLock = (Lock*)serverLockTable.Get(lockID);
				myLock->Release();
				printf("Server Releasing lock %d \n", lockID);
				replyMessage = "LRS";
					break;
				default:break;
			}
		}
		else if(buffer[0] == 'C')
		{
			/* Conditon operation */
			switch(buffer[1])
			{
				case 'C':
				myCond = new Condition("New Condition");
				condID = serverCondTable.Put(myCond);
				printf("Server Creating condition %d \n", condID);
				sprintf(tempMessage, "%d", condID);
				replyMessage = tempMessage;
					break;
				case 'D':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				condID = atoi(tempMessage);
				serverCondTable.Remove(condID);
				printf("Server Destroying condition %d \n", condID);
				replyMessage = "CDS";
					break;
				case 'S':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				tempMessage = strtok(NULL, delims);
				condID = atoi(tempMessage);
				myCond = (Condition*)serverCondTable.Get(condID);
				myLock = (Lock*)serverLockTable.Get(lockID);
				myCond->Signal(myLock);
				printf("Server Signaling condition %d with lock %d \n", condID, lockID);
				replyMessage = "CSS";
					break;
				case 'W':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				tempMessage = strtok(NULL, delims);
				condID = atoi(tempMessage);
				myCond = (Condition*)serverCondTable.Get(condID);
				myLock = (Lock*)serverLockTable.Get(lockID);
				myCond->Wait(myLock);
				printf("Server Waiting condition %d with lock %d \n", condID, lockID);
				replyMessage = "CWS";
					break;
				case 'B':
				strtok(buffer, delims);
				tempMessage = strtok(NULL, delims);
				lockID = atoi(tempMessage);
				tempMessage = strtok(NULL, delims);
				condID = atoi(tempMessage);
				myCond = (Condition*)serverCondTable.Get(condID);
				myLock = (Lock*)serverLockTable.Get(lockID);
				myCond->Broadcast(myLock);
				printf("Server Broadcasting condition %d with lock %d \n", condID, lockID);
				replyMessage = "CBS";
					break;
				default:break;
			}
		}
		

		/* create the reply package and send */
		outPktHdr.to = inPktHdr.from;
		outMailHdr.to = inMailHdr.from;
		outMailHdr.from = 1;
		outMailHdr.length = strlen(replyMessage)+1;
		success = postOffice->Send(outPktHdr, outMailHdr, replyMessage);

		if(!success)
		{
			printf("Server send reply message failed. Client probably not found. Terminating nachos.\n");
			interrupt->Halt();
		}
	}

    interrupt->Halt();
}
