#include "string.h"
#include "syscall.h"

typedef enum{ true,false }bool;
typedef enum{ app,pic,pass,cash}clerkType;
typedef int Lock; 
typedef int Condition; 


#define NUM_OF_CLERK 4
#define NUM_OF_CUSTOMER 10

int totalFinishCustomer = 0;
int totalFinishCustomerLock;

int customerIDCount = 0;
int applicationClerkIDCount=0;
int pictureClerkIDCount=0;
int passportClerkIDCount=0;
int cashierClerkIDCount=0;


int customerIDLock;
int applicationClerkLock;
int pictureClerkLock;
int passportClerkLock;
int cashierClerkLock;


void customerRun();
void pictureClerkRun();
void passportClerkRun();
void cashierClerkRun();
void managerRun();
void Initialize();
void applicationClerkRun();
typedef struct
{
	int customerID;
	int money;
	int applicationDone;
	int pictureDone;
	int passportDone;
	int cashierDone;
}Customer;

typedef struct
{
	int clerkID;
	int responsibleTable;
	int status;
}Clerk;

typedef struct
{
	int tableID;
	int tableMoney;
	int clerkCount;
	int leaveCount;
	int needClerk;
	int tableLock;
	int tableCond;
}cTable;

typedef struct
{
	int status;
	int ID;
}Spot;

typedef struct
{
	int lineID;
	int preferLineCount;
	int regLineCount;
	int money;
	Spot preferLine[NUM_OF_CUSTOMER];
	Spot regLine[NUM_OF_CUSTOMER];
	int preferLineLock;
	int regLineLock;
	int preferLineCond;
	int regLineCond;
}cLine;


cTable applicationTable = {0, 0, 0, 0, 0};
cTable pictureTable = {1, 0, 0, 0, 0};
cTable passportTable = {2, 0, 0, 0, 0};
cTable cashierTable = {3, 0, 0, 0, 0};

cLine applicationLine = {0, 0, 0, 0};
cLine pictureLine = {1, 0, 0, 0};
cLine passportLine = {2, 0, 0, 0};
cLine cashierLine = {3, 0, 0, 0};

Customer customerArray[NUM_OF_CUSTOMER];
Clerk applicationClerkArray[NUM_OF_CLERK];
Clerk pictureClerkArray[NUM_OF_CLERK];
Clerk passportClerkArray[NUM_OF_CLERK];
Clerk cashierClerkArray[NUM_OF_CLERK];


void customerRun()
{
	int myOwnID = -1;
	int i;
	
	/*get the customer ID*/
	Acquire(customerIDLock);
	myOwnID = customerIDCount;
	customerIDCount++;
	Release(customerIDLock);
	printf("[CUST] Customer %d start running \n", myOwnID);
	
	
	/* goto application line first*/
	printf("[CUST] Customer %d go queue up at applicaiton line \n", myOwnID);

	if(applicationLine.preferLineCount <= applicationLine.regLineCount)
	{
		/* add itself to prefer line*/
		Acquire(applicationLine.preferLineLock);
		customerArray[myOwnID].money -= 500;
		applicationLine.money += 500;
		applicationLine.preferLineCount += 1;
		printf("[CUST] Customer %d have joined the app prefer Line \n", myOwnID);
		Wait(applicationLine.preferLineLock, applicationLine.preferLineCond);
		Release(applicationLine.preferLineLock);
	}
	else
	{
		/*add itself to regular line*/
		Acquire(applicationLine.regLineLock);
		applicationLine.regLineCount += 1;
		printf("[CUST] Customer %d have joined the app regular Line \n", myOwnID);
		Wait(applicationLine.regLineLock, applicationLine.regLineCond);
		Release(applicationLine.regLineLock);
	}
	
	/*application complete*/
	customerArray[myOwnID].applicationDone = 1;
	printf("[CUST] Customer %d have completed application", myOwnID);

	/*customer now goes to picture line*/
	printf("[CUST] Customer %d go queue up at picture line \n", myOwnID);

	if(pictureLine.preferLineCount <= pictureLine.regLineCount)
	{
		/* add itself to prefer line*/
		Acquire(pictureLine.preferLineLock);
		customerArray[myOwnID].money -= 500;
		pictureLine.money += 500;
		pictureLine.preferLineCount += 1;
		printf("[CUST] Customer %d have joined the pic prefer Line \n", myOwnID);
		Wait(pictureLine.preferLineLock, pictureLine.preferLineCond);
		Release(pictureLine.preferLineLock);
	}
	else
	{
		/*add itself to regular line*/
		Acquire(pictureLine.regLineLock);
		pictureLine.regLineCount += 1;
		printf("[CUST] Customer %d have joined the pic regular Line \n", myOwnID);
		Wait(pictureLine.regLineLock, pictureLine.regLineCond);
		Release(pictureLine.regLineLock);
	}
	
	/*picture complete*/
	customerArray[myOwnID].pictureDone = 1;
	printf("[CUST] Customer %d have completed picture", myOwnID);
	
	while(customerArray[myOwnID].passportDone == 0)
	{
		/*customer now goes to passport line*/
		printf("[CUST] Customer %d go queue up at passport line \n", myOwnID);

		if(passportLine.preferLineCount <= passportLine.regLineCount)
		{
			/* add itself to prefer line*/
			Acquire(passportLine.preferLineLock);
			customerArray[myOwnID].money -= 500;
			passportLine.money += 500;
			passportLine.preferLineCount += 1;
			printf("[CUST] Customer %d have joined the pass prefer Line \n", myOwnID);
			Wait(passportLine.preferLineLock, passportLine.preferLineCond);
			Release(passportLine.preferLineLock);
		}
		else
		{
			/*add itself to regular line*/
			Acquire(passportLine.regLineLock);
			passportLine.regLineCount += 1;
			printf("[CUST] Customer %d have joined the pass regular Line \n", myOwnID);
			Wait(passportLine.regLineLock, passportLine.regLineCond);
			Release(passportLine.regLineLock);
		}
		
		/*picture complete*/
		if(customerArray[myOwnID].applicationDone == 1 && customerArray[myOwnID].pictureDone == 1)
		{
			customerArray[myOwnID].passportDone = 1;
			printf("[CUST] Customer %d have completed passport", myOwnID);
		}
	}
	
	/*customer now goes to cashier line*/
	printf("[CUST] Customer %d go queue up at cashier line \n", myOwnID);

	if(cashierLine.preferLineCount <= cashierLine.regLineCount)
	{
		/* add itself to prefer line*/
		Acquire(cashierLine.preferLineLock);
		customerArray[myOwnID].money -= 500;
		cashierLine.money += 500;
		cashierLine.preferLineCount += 1;
		printf("[CUST] Customer %d have joined the cash prefer Line \n", myOwnID);
		Wait(cashierLine.preferLineLock, cashierLine.preferLineCond);
		Release(cashierLine.preferLineLock);
	}
	else
	{
		/*add itself to regular line*/
		Acquire(cashierLine.regLineLock);
		cashierLine.regLineCount += 1;
		printf("[CUST] Customer %d have joined the cash regular Line \n", myOwnID);
		Wait(cashierLine.regLineLock, cashierLine.regLineCond);
		Release(cashierLine.regLineLock);
	}
	
	/*cashier complete*/
	customerArray[myOwnID].cashierDone = 1;
	printf("[CUST] Customer %d have completed cashier", myOwnID);
	
	Exit(0);
}


void pictureClerkRun()
{

   int clerkID = -1;
   int c = 0;//customer
   int type = 0;//Pic type = 0;
   //Get Picture Clerk ID
   Acquire(pictureClerkLock);
   clerkID = pictureClerkIDCount ++;
   Release(pictureClerkLock);
   
	printf("[Clerk]PicClerk %d Start Running \n",clerkID);
   while(1)
   {
      //Do Lock
      Acquire(pictureLine.preferLineLock);
      printf("[Clerk]PicClerk %d Get Pic line lock\n",clerkID);
      if(pictureLine.preferLineCount != 0) //if there is customer in prefer line
      {
         printf("[Clerk]PicClerk %d check prefer line\n",clerkID);
         pictureLine.preferLineCount --;
         Signal(pictureLine.preferLineLock,pictureLine.preferLineCond); 
         printf("[Clerk]PicClerk %d Release Pic line lock\n",clerkID);
         Release(pictureLine.preferLineLock);
         Yield();
      }else{
         Release(pictureLine.preferLineLock);
         Acquire(pictureLine.regLineLock);

         if(pictureLine.regLineCount != 0)
         {
            printf("[Clerk]PicClerk %d check reg line \n",clerkID);
            pictureLine.regLineCount --;
            Signal(pictureLine.regLineLock,pictureLine.regLineCond); 
            Release(pictureLine.regLineLock);
            Yield();
         } else {
            Release(pictureLine.regLineLock);
            //Go Sleep
            printf("[Clerk]PicClerk %d leave table now\n",clerkID);
            Acquire(pictureTable.tableLock);
            Wait(pictureTable.tableLock,pictureTable.tableCond);
            Release(pictureTable.tableLock);
            printf("[Clerk]PicClerk %d now come back to table \n",clerkID);
         }
      } 
	}//while(1);

	Exit(0);
}

void passportClerkRun()
{
   int clerkID = -1;
   int c = 0;//customer
   int type = 0;//Passport type = 0;
   //Get Passportture Clerk ID
   Acquire(passportClerkLock);
   clerkID = passportClerkIDCount ++;
   Release(passportClerkLock);
   
	printf("[Clerk]PassportClerk %d Start Running \n",clerkID);
   while(1)
   {
      //Do Lock
      Acquire(passportLine.preferLineLock);
      printf("[Clerk]PassportClerk %d Get Passport line lock\n",clerkID);
      if(passportLine.preferLineCount != 0) //if there is customer in prefer line
      {
         printf("[Clerk]PassportClerk %d check prefer line\n",clerkID);
         passportLine.preferLineCount --;
         Signal(passportLine.preferLineLock,passportLine.preferLineCond); 
         printf("[Clerk]PassportClerk %d Release Passport line lock\n",clerkID);
         Release(passportLine.preferLineLock);
         Yield();
      }else{
         Release(passportLine.preferLineLock);
         Acquire(passportLine.regLineLock);

         if(passportLine.regLineCount != 0)
         {
            printf("[Clerk]PassportClerk %d check reg line \n",clerkID);
            passportLine.regLineCount --;
            Signal(passportLine.regLineLock,passportLine.regLineCond); 
            Release(passportLine.regLineLock);
            Yield();
         } else {
            Release(passportLine.regLineLock);
            //Go Sleep
            printf("[Clerk]PassportClerk %d leave table now\n",clerkID);
            Acquire(passportTable.tableLock);
            Wait(passportTable.tableLock,passportTable.tableCond);
            Release(passportTable.tableLock);
            printf("[Clerk]PassportClerk %d now come back to table \n",clerkID);
         }
      } 
	}//while(1);

	Exit(0);
}

void cashierClerkRun()
{
   int clerkID = -1;
   int c = 0;//customer
   int type = 0;//cashier type = 0;
   //Get cashier Clerk ID
   Acquire(cashierClerkLock);
   clerkID = cashierClerkIDCount ++;
   Release(cashierClerkLock);
   
	printf("[Clerk]cashierClerk %d Start Running \n",clerkID);
   while(1)
   {
      //Do Lock
      Acquire(cashierLine.preferLineLock);
      printf("[Clerk]cashierClerk %d Get cashier line lock\n",clerkID);
      if(cashierLine.preferLineCount != 0) //if there is customer in prefer line
      {
         printf("[Clerk]cashierClerk %d check prefer line\n",clerkID);
         cashierLine.preferLineCount --;
         Signal(cashierLine.preferLineLock,cashierLine.preferLineCond); 
         printf("[Clerk]cashierClerk %d Release cashier line lock\n",clerkID);
         Release(cashierLine.preferLineLock);
         Yield();
      }else{
         Release(cashierLine.preferLineLock);
         Acquire(cashierLine.regLineLock);

         if(cashierLine.regLineCount != 0)
         {
            printf("[Clerk]cashierClerk %d check reg line \n",clerkID);
            cashierLine.regLineCount --;
            Signal(cashierLine.regLineLock,cashierLine.regLineCond); 
            Release(cashierLine.regLineLock);
            Yield();
         } else {
            Release(cashierLine.regLineLock);
            //Go Sleep
            printf("[Clerk]cashierClerk %d leave table now\n",clerkID);
            Acquire(cashierTable.tableLock);
            Wait(cashierTable.tableLock,cashierTable.tableCond);
            Release(cashierTable.tableLock);
            printf("[Clerk]cashierClerk %d now come back to table \n",clerkID);
         }
      } 
	}//while(1);

	Exit(0);
}

void managerRun()
{
	
}

void Initialize()
{
	int i;
	
	/*  Create all required lock and condition for table, and line for passport office*/
	
	totalFinishCustomerLock = CreateLock();
	
	applicationTable.tableLock = CreateLock();
	pictureTable.tableLock = CreateLock();
	passportTable.tableLock = CreateLock();
	cashierTable.tableLock = CreateLock();

	customerIDLock = CreateLock();
	applicationClerkLock = CreateLock();
	pictureClerkLock = CreateLock();
	passportClerkLock = CreateLock();
	cashierClerkLock = CreateLock();
	
	applicationTable.tableCond = CreateCondition();
	pictureTable.tableCond = CreateCondition();
	passportTable.tableCond = CreateCondition();
	cashierTable.tableCond = CreateCondition();
	
	applicationLine.regLineLock = CreateLock();
	applicationLine.preferLineLock = CreateLock();
	applicationLine.regLineCond = CreateCondition();
	applicationLine.preferLineCond = CreateCondition();
	
	pictureLine.regLineLock = CreateLock();
	pictureLine.preferLineLock = CreateLock();
	pictureLine.regLineCond = CreateCondition();
	pictureLine.preferLineCond = CreateCondition();
	
	passportLine.regLineLock = CreateLock();
	passportLine.preferLineLock = CreateLock();
	passportLine.regLineCond = CreateCondition();
	passportLine.preferLineCond = CreateCondition();
	
	cashierLine.regLineLock = CreateLock();
	cashierLine.preferLineLock = CreateLock();
	cashierLine.regLineCond = CreateCondition();
	cashierLine.preferLineCond = CreateCondition();
	
	/*Initialize values for the four lines, clean them up*/
	for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		applicationLine.regLine[i].ID = -1;
		applicationLine.regLine[i].status = 0;
		applicationLine.preferLine[i].ID = -1;
		applicationLine.preferLine[i].status = 0;
	}
	for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		pictureLine.regLine[i].ID = -1;
		pictureLine.regLine[i].status = 0;
		pictureLine.preferLine[i].ID = -1;
		pictureLine.preferLine[i].status = 0;
	}for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		passportLine.regLine[i].ID = -1;
		passportLine.regLine[i].status = 0;
		passportLine.preferLine[i].ID = -1;
		passportLine.preferLine[i].status = 0;
	}for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		cashierLine.regLine[i].ID = -1;
		cashierLine.regLine[i].status = 0;
		cashierLine.preferLine[i].ID = -1;
		cashierLine.preferLine[i].status = 0;
	}
	
	/*Creating Customers*/
	for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		customerArray[i].money = 1600;
		customerArray[i].applicationDone = 0;
		customerArray[i].pictureDone = 0;
		customerArray[i].passportDone = 0;
		customerArray[i].cashierDone = 0;
		Fork(customerRun);
	}
	
	/* Creating Clerks for application table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		applicationClerkArray[i].responsibleTable = 0;
		Fork(applicationClerkRun);
	}
	
	/* Creating Clerks for picture table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		pictureClerkArray[i].responsibleTable = 1;
		Fork(pictureClerkRun);
	}
	
	/* Creating Clerks for passport table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		passportClerkArray[i].responsibleTable = 2;
		Fork(passportClerkRun);
	}
	
	/* Creating Clerks for cashier table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		cashierClerkArray[i].responsibleTable = 3;
		Fork(pictureClerkRun);
	}
	
	/*Create Manager*/
	
	
}
void handleCustomer(int c,int type){
   // Type 0 : Application Clerk
   // Type 1 : Picture Clerk
   // Type 2 : Clerk
   // Type 3 : Application Clerk
   switch(type)
   {
   case 0:
      customerArray[c].applicationDone = 1;
      break;
   case 1:
      customerArray[c].pictureDone= 1;
      break;
   case 2:
      customerArray[c].passportDone= 1;
      break;
   case 3:
      customerArray[c].cashierDone= 1;
      break;
      
   }

}
int getCustomerID(int l,int PreOrReg){
   // PreOrReg 0 : prefer line 
   // PreOrReg 1 : regular line
   cLine *line = (cLine*)l;
   int i;
   int customerID = -1;
  for(i = 0 ; i < NUM_OF_CUSTOMER ; i++){
   if(PreOrReg == 0)
   {
      //check prefer line
     if(line->preferLine[i].status == 1){
      //get id than set spot to empty
      customerID = line->preferLine[i].ID;
      line->preferLine[i].status = 0;
      line->preferLineCount --;
      break;
     } 
      
   }else{
      //check regular line
     if(line->regLine[i].status == 1){
      //get id than set spot to empty
      customerID = line->regLine[i].ID;
      line->regLine[i].status = 0;
      line->regLineCount --;
      break;
     } 
   }//end PreOrReg
  
  }//end for 
  return customerID;
}   
void applicationClerkRun()
{

   int clerkID = -1;
   int c = 0;//customer
   int type = 0;//App type = 0;
   //Get Application Clerk ID
   Acquire(applicationClerkLock);
   clerkID = applicationClerkIDCount ++;
   Release(applicationClerkLock);
   
	printf("[Clerk]AppClerk %d Start Running \n",clerkID);
   while(1)
   {
      //Do Lock
      Acquire(applicationLine.preferLineLock);
      printf("[Clerk]AppClerk %d Get app line lock\n",clerkID);
      if(applicationLine.preferLineCount != 0) //if there is customer in prefer line
      {
         printf("[Clerk]AppClerk %d check prefer line\n",clerkID);
         applicationLine.preferLineCount --;
         Signal(applicationLine.preferLineLock,applicationLine.preferLineCond); 
         printf("[Clerk]AppClerk %d Release app line lock\n",clerkID);
         Release(applicationLine.preferLineLock);
         Yield();
      }else{
         Release(applicationLine.preferLineLock);
         Acquire(applicationLine.regLineLock);

         if(applicationLine.regLineCount != 0)
         {
            printf("[Clerk]AppClerk %d check reg line \n",clerkID);
            applicationLine.regLineCount --;
            Signal(applicationLine.regLineLock,applicationLine.regLineCond); 
            Release(applicationLine.regLineLock);
            Yield();
         } else {
            Release(applicationLine.regLineLock);
            //Go Sleep
            printf("[Clerk]AppClerk %d leave table now\n",clerkID);
            Acquire(applicationTable.tableLock);
            Wait(applicationTable.tableLock,applicationTable.tableCond);
            Release(applicationTable.tableLock);
            printf("[Clerk]AppClerk %d now come back to table \n",clerkID);
         }
      } 
	}//while(1);

	Exit(0);
}
void main()
{
	Initialize();
	
	
	/* Destroy all created lock and condition */
//	DestroyLock(applicationLine.preferLineLock);
//	DestroyLock(applicationLine.regLineLock);
//	DestroyLock(pictureLine.preferLineLock);
//	DestroyLock(pictureLine.regLineLock);
//	DestroyLock(passportLine.preferLineLock);
//	DestroyLock(passportLine.regLineLock);
//	DestroyLock(cashierLine.preferLineLock);
//	DestroyLock(cashierLine.regLineLock);
	
//	DestroyLock(applicationTable.tableLock);
//	DestroyLock(pictureTable.tableLock);
//	DestroyLock(passportTable.tableLock);
//	DestroyLock(cashierTable.tableLock);
//	
//	DestroyCondition(applicationTable.tableCond);
//	DestroyCondition(pictureTable.tableCond);
//	DestroyCondition(passportTable.tableCond);
//	DestroyCondition(cashierTable.tableCond);
	
	Exit(0);
}
