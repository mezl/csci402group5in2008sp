#include "string.h"
#include "syscall.h"

typedef enum{ true,false }bool;
typedef int Lock; 
typedef int Condition; 


#define NUM_OF_CLERK 4
#define NUM_OF_CUSTOMER 10

int applicationLineLock;
int pictureLineLock;
int passportLineLock;
int cashierLineLock;

int applicationTableLock;
int pictureTableLock;
int passportTableLock;
int cashierTableLock;

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

int applicationTableCond;
int pictureTableCond;
int passportTableCond;
int cashierTableCond;


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
}cTable;

typedef struct
{
	int lineID;
	int preferLineCount;
	int regLineCount;
	int preferLineQueue;
	int regLineQueue;
	int money;
	
}cLine;


cTable applicationTable;
cTable pictureTable;
cTable passportTable;
cTable cashierTable;

cLine applicationLine;
cLine pictureLine;
cLine passportLine;
cLine cashierLine;

Customer customerArray[NUM_OF_CUSTOMER];
Clerk applicationClerkArray[NUM_OF_CLERK];
Clerk pictureClerkArray[NUM_OF_CLERK];
Clerk passportClerkArray[NUM_OF_CLERK];
Clerk cashierClerkArray[NUM_OF_CLERK];




void customerRun(Customer myCustomer)
{
	Exit(0);
}

void clerkRun(Clerk myClerk)
{
	Exit(0);
}

void managerRun()
{
	
}

void Initialize()
{
	int i;
	
	/*  Create all required lock and condition for table, and line for passport office*/
	applicationLineLock = CreateLock();
	pictureLineLock = CreateLock();
	passportLineLock = CreateLock();
	cashierLineLock = CreateLock();
	
	applicationTableLock = CreateLock();
	pictureTableLock = CreateLock();
	passportTableLock = CreateLock();
	cashierTableLock = CreateLock();

	customerIDLock = CreateLock();
	applicationClerkLock = CreateLock();
	pictureClerkLock = CreateLock();
	passportClerkLock = CreateLock();
	cashierClerkLock = CreateLock();
	
	applicationTableCond = CreateCondition();
	pictureTableCond = CreateCondition();
	passportTableCond = CreateCondition();
	cashierTableCond = CreateCondition();
	
	
	applicationTable = {0, 0, 0, 0, 0};
	pictureTable = {1, 0, 0, 0, 0};
	passportTable = {2, 0, 0, 0, 0};
	cashierTable = {3, 0, 0, 0, 0};

	applicationLine = {0, 0, 0, 0, 0, 0};
	pictureLine = {1, 0, 0, 0, 0, 0};
	passportLine = {2, 0, 0, 0, 0, 0};
	cashierLine = {3, 0, 0, 0, 0, 0};
	
	/* Create Manager, Customer and Clerk threads */
	
	/* Creating Clerks for application table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		applicationClerkArray[i].clerkID = i;
		applicationClerkArray[i].responsibleTable = 0;
		applicationClerkArray[i].status = 1;
	}
	
	/* Creating Clerks for picture table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		pictureClerkArray[i].clerkID = i;
		pictureClerkArray[i].responsibleTable = 0;
		pictureClerkArray[i].status = 1;
	}
	
	/* Creating Clerks for passport table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		passportClerkArray[i].clerkID = i;
		passportClerkArray[i].responsibleTable = 0;
		passportClerkArray[i].status = 1;
	}
	
	/* Creating Clerks for cashier table*/
	for(i=0; i<NUM_OF_CLERK; i++)
	{
		cashierClerkArray[i].clerkID = i;
		cashierClerkArray[i].responsibleTable = 0;
		cashierClerkArray[i].status = 1;
	}
	
	/*Creating Customers*/
	for(i=0; i<NUM_OF_CUSTOMER; i++)
	{
		
	}	
	
	/*Create Manager*/
	
	
}

void main()
{
	Initialize();
	
	
	/* Destroy all created lock and condition */
	DestroyLock(applicationLineLock);
	DestroyLock(pictureLineLock);
	DestroyLock(passportLineLock);
	DestroyLock(cashierLineLock);
	
	DestroyLock(applicationTableLock);
	DestroyLock(pictureTableLock);
	DestroyLock(passportTableLock);
	DestroyLock(cashierTableLock);
	
	DestroyCondition(applicationTableCond);
	DestroyCondition(pictureTableCond);
	DestroyCondition(passportTableCond);
	DestroyCondition(cashierTableCond);
	
	Exit(0);
}
