/* 
test for lock and condition system calls 
main shall first fork func1 and waits to be signaled by it. 
main then forks func2 and waits to be signaled by it.
func1 and func2 both wait to be broadcasted by main before they exist

the write syscalls to the console print out MSG that describe the desired 
synchonization described above. 
*/

//Set how many customer and clerk want to run
#define CUSTOMER_NUM 4
#define CLERK_NUM 5
#define CUSTOMER
#define APPLICATION 
#define PICTURE
//#define PASSPORT
//#define CASHIER 
//#define MANAGER
//#define CHECK_LINE_3
//#define LINE_CHECK	
//#define SHOW_MONEY	

#include "syscall.h"
#include "string.h"
  int lock1; 
  int lock2; 
  int cond1;
  int cond2;
  int cond3;
typedef enum{ true,false }bool;
typedef int Lock; 
typedef int Condition; 
typedef struct  {
//		cTable(int ID_in, int initialClerkCount);
//		int clerkCount();
//		void addClerk(char *name,int id,bool display = false);
//		void leaveTable(char *name,int id,bool display = false);
//		void acquireLock(char *name,int id,bool display = false);
//		void releaseLock(char *name,int id,bool display = false);
//		void addMoney(int amount);
//		int reportMoney();

		int tableID;
		int tableMoney;
		int cCount;
		int leaveCount;
		int tableLock;
		Lock tableMoneyLock;
		Condition tableCondition;
		int needClerk;
}cTable;
typedef struct {
//		cLine(char *name,int ID);
//		void addPreferLine(int c,int mount);
//		void addRegLine(int c);
//		void * getNextPreferLineCustomer(char *clerkName,int clerkID);
//		void * getNextRegLineCustomer(char *clerkName,int clerkID);
//		int preferCustomerCount(){return preferLineCount;}
//		int regCustomerCount(){return regLineCount;}
//		void preferAcquire(char *name = "NoName",int id = 0,bool display=false);
//		void preferRelease(char *name = "NoName",int id = 0,bool display=false);
//		void regAcquire(char *name,int id = 0,bool display=false);
//		void regRelease(char *name,int id =0 ,bool display=false);
//		void Acquire(char *name,int id = 0,bool display=false);
//		void Release(char *name,int id =0 ,bool display=false);
//		bool IsRegLineEmpty();
//		bool IsPreferLineEmpty();
//		int getID(){return lineID;}
//		char *getName(){return lineName;}
//		int reportMoney();
//		bool nobody();

		bool preferNeedClerk;//bool
		bool regNeedClerk;//bool
		int preferLineCount;		
		int regLineCount;		
		int lineID;
		char *lineName;
		int amount;
		Condition preferLineCond;
		Lock preferLineLock; 
		Condition regLineCond;
		Lock regLineLock;//Lock
		Condition lineLock;//for lock both line
//		List* preferLineQueue;	
//		List* regLineQueue;	
		int callNext;
		int regCallNext;
}cLine;
typedef struct 
{
//	Clerk(cLine *l,cTable *t,int id,char *name);
//	virtual ~Clerk(){}
//	void run();
//	virtual void handleCustomer(Customer *c){
	cLine cline;
	cTable table;
	int getID;
	char *clerkName;
	Lock clerkLock;
	Condition clerkCondition;
}Clerk;
typedef struct 
{
//	void customerRun();
//	void gotoApplicationLine();
//	void gotoPictureLine();
//	void gotoPassportLine();
//	void gotoCashierLine();
//	void completeApplication();
//	void completePicture();
//	void completePassport();
//	void completeCashier();
//	bool checkApplication();
//	bool checkPicture();
//	bool checkPassport();
//	bool checkCashier();
//	void chargeMoney(int mount);
//	void punished();
//	int getID(){return customerID;}
//	char *getName(){return customerName;}
//	void setClerk(Lock *l,Condition *c){
//		setClerkLock->Acquire();
//		clerkSet = true;
//		clerkLock = l;
//		clerkCondition = c;
//		setClerkLock->Release();
//	}
//	bool getClerk(){
//		setClerkLock->Acquire();
//		if(!clerkSet)
//			printf("[CUST]%s%d say:no clerk can help me\n",customerName,customerID);
//		setClerkLock->Release();
//		return clerkSet;
//	
//	}
//	void wait(){
//		customerLock->Acquire();
//		customerCondition->Wait(customerLock);
//		customerLock->Release();	
//	}
//	void wakeup(){
//		customerLock->Acquire();
//		customerCondition->Signal(customerLock);
//		customerLock->Release();	
//	}
	int getID;
	char *customerName;
	int money;
	int applicationLine;
	int pictureLine;
	int passportLine;
	int cashierLine;
	bool applicationDone;
	bool pictureDone;
	bool passportDone;
	bool cashierDone;
	bool clerkSet;
	Lock setClerkLock;//given by free clerk
	Lock clerkLock;//given by free clerk
	Lock customerLock;//given by free clerk
	Condition clerkCondition;//given by free clerk
	Condition customerCondition;
}Customer;
// --------------------------------------------------
//Main program
// --------------------------------------------------
#ifdef CUSTOMER
	Customer customer[CUSTOMER_NUM];
#endif
#ifdef APPLICATION	
	Clerk appClerk[CLERK_NUM];
#endif
#ifdef PICTURE	
	Clerk picClerk[CLERK_NUM];
#endif
#ifdef PASSPORT
	Clerk passClerk[CLERK_NUM];
#endif
#ifdef CASHIER	
	Clerk cashClerk[CLERK_NUM];
#endif

Condition newCondition(char * name){
   return 0;
}         
Lock newLock(char * name){
   return 0;
}         
void newCustomer(int cus,char *name, int ID_in, int money_in 
//			int applicationLine_in, 
//			int pictureLine_in, 
//			int passportLine_in, 
//			int cashierLine_in
         )
{
   Customer *c = (Customer*)cus;
	printf("[CUST]Build Customer %d\n",ID_in);
	c->getID = ID_in;
	//c.customerName = name;
	c->money = money_in;
//	c.applicationLine = applicationLine_in;
//	c.pictureLine = pictureLine_in;
//	c.passportLine = passportLine_in;
//	c.cashierLine = cashierLine_in;

	c->applicationDone = false;
	c->pictureDone = false;
	c->passportDone = false;
	c->cashierDone = false;
	c->clerkSet = false;
	c->customerCondition = newCondition("customer");
	c->setClerkLock = newLock("SetClerkLock");//given by free clerk
	c->customerLock = newLock("Customer Lock");//given by free clerk
	//random function initilization
	//srand(time(NULL));

}         
void newClerk(int ck,int id,char *name)
{

   Clerk *c = (Clerk*)ck;
//	cline = l;
//	table = t;
//share with manager
	c->getID= id;
	c->clerkName = name;
	c->clerkLock = newLock(name);
	c->clerkCondition = newCondition(name);
}
void office()
{
   int i;
	printf("[Office]Start Office Sim\n");
#ifdef MANAGER
	// create the manager
	printf("[Office]Create Manager\n");
	Timer *t = new Timer(managerHandler, 0, false);
#endif

#ifdef CUSTOMER
	printf("[Office]Create Customer \n");
	//Thread *customer_thread[CUSTOMER_NUM];
	for(i = 0; i < CUSTOMER_NUM; i++){
		newCustomer((int)&customer[i], "customer",i, 600); 
      //applicationLine, pictureLine, passportLine, cashierLine);
		printf("[Office]Create Customer %d Thread\n",customer[i].getID);
		//customer_thread[i] = new Thread(msg);
		printf("[Office]Fork Customer %d Thread\n",customer[i].getID);
		//customer_thread[i] -> Fork(myCustomerForkFunc, (int)customer[i]);
	}
#endif
#ifdef APPLICATION	
	// create app clerks ( clerk for each table/job)
	//Thread *appClerk_thread[CLERK_NUM];
	for(i = 0; i < CLERK_NUM; i++){
		newClerk((int)&appClerk[i],i,"AppClerk");
		printf("[Office]Create AppClerk %d Thread\n",appClerk[i].getID);
		
		//appClerk_thread[i] = new Thread("AppClerk");
		printf("[Office]Fork AppClerk %d Thread\n",appClerk[i].getID);
		//appClerk_thread[i] -> Fork(myClerkForkFunc, (int)appClerk[i]);
	}
#endif
#ifdef PICTURE	
	// create pic clerks ( clerk for each table/job)
	//Thread *picClerk_thread[CLERK_NUM];
	for(i = 0; i < CLERK_NUM; i++){
		newClerk((int)&picClerk[i],i,"PicClerk");
		printf("[Office]Create PicClerk %d Thread\n",picClerk[i].getID);
		
		//picClerk_thread[i] = new Thread("PicClerk");
		printf("[Office]Fork PicClerk %d Thread\n",picClerk[i].getID);
		//picClerk_thread[i] -> Fork(myClerkForkFunc, (int)picClerk[i]);
	}
#endif 	
#ifdef PASSPORT
	// create passport clerks ( clerk for each table/job)
	//Thread *passClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		newClerk((int)&passClerk[i],i,"PassClerk");
		printf("[Office]Create PassClerk %d Thread\n",passClerk[i]->getID());
		
		//passClerk_thread[i] = new Thread("PassClerk");
		printf("[Office]Fork PassClerk %d Thread\n",passClerk[i]->getID());
		//passClerk_thread[i] -> Fork(myClerkForkFunc, (int)passClerk[i]);
	}
#endif
#ifdef CASHIER	
	// create cashier clerks ( clerk for each table/job)
	//Thread *cashClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		newClerk((int)&cashClerk[i],i,"CashClerk");
		printf("[Office]Create CashClerk %d Thread\n",cashClerk[i]->getID());
		
		//cashClerk_thread[i] = new Thread("CashClerk");
		printf("[Office]Fork CashClerk %d Thread\n",cashClerk[i]->getID());
		//cashClerk_thread[i] -> Fork(myClerkForkFunc, (int)cashClerk[i]);
	}
#endif

}

int main()
{
   int num = 31;
   office();
  printf("[main]printf test !!!![%d] start %d [%s]sdfsd\n",num,180,"abcdr");


  println("[main]main exits\n");

  Exit(0);
}


//	Simple test cases for the threads assignment.
//
/*
#include "copyright.h"
#include "../threads/system.h"
#ifdef CHANGED
#include "../threads/synch.h"
#endif
#ifdef CHANGED
///////////////////////////////////////////////////////////////////////////////
// Part 3
//
//Enter function are Office
//use the define below can turn on/off code
//
//
///////////////////////////////////////////////////////////////////////////////
#include "passportOffice.h"
#include "manager.cc"
#include "customer.h"
#include "clerk.h"
#include "cline.h"
#include "ctable.h"
#include "utility.h"
#include <timer.h>
#include "appclerk.cc"
#include "picclerk.cc"
#include "passclerk.cc"
#include "cashclerk.cc"

////////////////////////////////////////////////
#define CUSTOMER
#define APPLICATION 
#define PICTURE
#define PASSPORT
#define CASHIER 
#define MANAGER
//#define CHECK_LINE_3
//#define LINE_CHECK	
//#define SHOW_MONEY	
////////////////////////////////////////////////
cLine *applicationLine = new cLine("app line",1);
cLine *pictureLine     = new cLine("pic line",2);
cLine *passportLine    = new cLine("passport line",3);
cLine *cashierLine     = new cLine("cashier line",4);

cTable *applicationTable = new cTable(1, CLERK_NUM);
cTable *pictureTable     = new cTable(2, CLERK_NUM);
cTable *passportTable    = new cTable(3, CLERK_NUM);
cTable *cashierTable     = new cTable(4, CLERK_NUM);


void Manager(int x);
void managerHandler(int x);
void myCustomerForkFunc(int x);
void myClerkForkFunc(int x);
void office();

void myCustomerForkFunc(int x)
{
	Customer *c = (Customer*)x;
	printf("[custFork]Customer %d run\n",c->getID());
	c -> customerRun();
	delete c;
}

void myClerkForkFunc(int x)
{
	Clerk *cl = (Clerk*)x;
	printf("[ckerkFork]%s %d run\n",cl->getName(),cl->getID());
	cl -> run();
	delete cl;
}

// --------------------------------------------------
//Main program
// --------------------------------------------------
void office()
{
	printf("[Office]Start Office Sim\n");
#ifdef MANAGER
	// create the manager
	printf("[Office]Create Manager\n");
	Timer *t = new Timer(managerHandler, 0, false);
#endif

#ifdef CUSTOMER
	printf("[Office]Create Customer \n");
	Customer *customer[CUSTOMER_NUM];
	Thread *customer_thread[CUSTOMER_NUM];
	for(int i = 0; i < CUSTOMER_NUM; i++){
		customer[i] = new Customer("customer",i, 600, applicationLine, pictureLine, passportLine, cashierLine);
		printf("[Office]Create Customer %d Thread\n",customer[i]->getID());
		char msg[12];
		sprintf(msg,"Customer %d",i);
		customer_thread[i] = new Thread(msg);
		printf("[Office]Fork Customer %d Thread\n",customer[i]->getID());
		customer_thread[i] -> Fork(myCustomerForkFunc, (int)customer[i]);
	}
#endif
#ifdef APPLICATION	
	// create app clerks ( clerk for each table/job)
	Clerk *appClerk[CLERK_NUM];
	Thread *appClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		appClerk[i] = new AppClerk(applicationLine,applicationTable,i,"AppClerk");
		printf("[Office]Create AppClerk %d Thread\n",appClerk[i]->getID());
		
		appClerk_thread[i] = new Thread("AppClerk");
		printf("[Office]Fork AppClerk %d Thread\n",appClerk[i]->getID());
		appClerk_thread[i] -> Fork(myClerkForkFunc, (int)appClerk[i]);
	
	}
#endif
#ifdef PICTURE	
	// create pic clerks ( clerk for each table/job)
	Clerk *picClerk[CLERK_NUM];
	Thread *picClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		picClerk[i] = new PicClerk(pictureLine,pictureTable,i,"PicClerk");
		printf("[Office]Create PicClerk %d Thread\n",picClerk[i]->getID());
		
		picClerk_thread[i] = new Thread("PicClerk");
		printf("[Office]Fork PicClerk %d Thread\n",picClerk[i]->getID());
		picClerk_thread[i] -> Fork(myClerkForkFunc, (int)picClerk[i]);
	}
#endif 	
#ifdef PASSPORT
	// create passport clerks ( clerk for each table/job)
	Clerk *passClerk[CLERK_NUM];
	Thread *passClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		passClerk[i] = new PassClerk(passportLine,passportTable,i,"PassClerk");
		printf("[Office]Create PassClerk %d Thread\n",passClerk[i]->getID());
		
		passClerk_thread[i] = new Thread("PassClerk");
		printf("[Office]Fork PassClerk %d Thread\n",passClerk[i]->getID());
		passClerk_thread[i] -> Fork(myClerkForkFunc, (int)passClerk[i]);
	}
#endif
#ifdef CASHIER	
	// create cashier clerks ( clerk for each table/job)
	Clerk *cashClerk[CLERK_NUM];
	Thread *cashClerk_thread[CLERK_NUM];
	for(int i = 0; i < CLERK_NUM; i++){
		cashClerk[i] = new CashClerk(cashierLine,cashierTable,i,"CashClerk");
		printf("[Office]Create CashClerk %d Thread\n",cashClerk[i]->getID());
		
		cashClerk_thread[i] = new Thread("CashClerk");
		printf("[Office]Fork CashClerk %d Thread\n",cashClerk[i]->getID());
		cashClerk_thread[i] -> Fork(myClerkForkFunc, (int)cashClerk[i]);
	}
#endif

}

bool appNeedClerk = true;
bool picNeedClerk = true;
bool passNeedClerk = true;
bool cashNeedClerk = true;

void lineCheck(int x){
#ifdef LINE_CHECK	
	bool display = true;

	if(applicationLine->nobody()){
		if(display)printf("[Manager]%d No body in App Line\n",x);
		appNeedClerk = false;
	}else{
	
		if(display)printf("[Manager]%d Some one body in App Line\n",x);
		appNeedClerk = true;
	}
	if(pictureLine->nobody()){
		if(display)printf("[Manager]%d No body in Pic Line\n",x);
		picNeedClerk = false;
	}else{
		if(display)printf("[Manager]%d Some one body in Pic Line\n",x);
		picNeedClerk = true;
	}
	if(passportLine->nobody()){
		if(display)printf("[Manager]%d No body in Pass Line\n",x);
		passNeedClerk = false;
	}else{
		if(display)printf("[Manager]%d Some one body in pass Line\n",x);
		passNeedClerk = true;
	}
	if(cashierLine->nobody()){
		if(display)printf("[Manager]%d No body in Cash Line\n",x);
		cashNeedClerk = false;
	}else{
		if(display)printf("[Manager]%d Some one body in Cash Line\n",x);
		cashNeedClerk = true;
	}
#endif
}
int managerHandlCount = 0;
int managerHandlCount1 = 0;
int managerCount = 0;
Lock managerLock("Manager Lock");
void managerHandler(int x)
{
	if(managerHandlCount == 1)
	{
		managerHandlCount = 0; 
		if(managerHandlCount1 == 1)
		{
			managerHandlCount1 = 0; 
			if(managerCount == 30000)
				managerCount = 0;
			char msg[20];
			sprintf(msg,"ManagerT %d",managerCount);
			Thread *manager_thread = new Thread(msg);
			manager_thread -> Fork(Manager, managerCount++);
			//delete manager_thread; 
		}else{

			managerHandlCount1 ++ ; 
		}
	}else{
		managerHandlCount ++ ; 
	}
}
void Manager(int x)
{
	managerLock.Acquire();

	bool display = false;//Show debug message or not


	// check each table for number of clerks
	// add 1 clerk if table is empty
	char *name = "Manager";
	//kainew check
	if(display)printf("[Manager]%d I am Starting.....\n",x);
	if(applicationLine->preferNeedClerk())
		applicationTable->addClerk(name,x,display);
	if(applicationLine->regNeedClerk())
		applicationTable->addClerk(name,x,display);
	if(pictureLine->preferNeedClerk())
		pictureTable->addClerk(name,x,display);
	if(pictureLine->regNeedClerk())
		pictureTable->addClerk(name,x,display);
	if(passportLine->preferNeedClerk())
		passportTable->addClerk(name,x,display);
	if(passportLine->regNeedClerk())
		passportTable->addClerk(name,x,display);
	if(cashierLine->preferNeedClerk())
		cashierTable->addClerk(name,x,display);
	if(cashierLine->regNeedClerk())
		cashierTable->addClerk(name,x,display);
	
/*	
#ifdef APPLICATION
	//applicationLine->Acquire(name, 0);
	applicationTable->acquireLock(name,x);
	lineCheck(x);
	if(display)printf("[Manager]%d Get appTable lock.....\n",x);
	// if no clerk work and some customer in line
	if ((applicationTable->clerkCount() == 0)&& appNeedClerk );//&& !applicationLine->nobody())
	{
		if(display)printf("[Manager]App Clerk %d ,Customer in Line \n",applicationTable->clerkCount());
		applicationTable->addClerk(name,x,display);
		if(display)printf("[Manager] wakeup a AppClerk to Application table\n");
	}
	applicationTable->releaseLock(name,x);
	//applicationLine->Release(name, 0);
#endif	
#ifdef PICTURE
	//pictureLine->Acquire(name, 0);
	pictureTable->acquireLock(name,x);
	lineCheck(x);
	if ((pictureTable->clerkCount() == 0)&& picNeedClerk);//&& !pictureLine->nobody())
	{
		if(display)printf("[Manager]Pic Clerk %d Customer in Line \n",pictureTable->clerkCount());
		pictureTable->addClerk(name,x,display);
		if(display)printf("[Manager] wakeup a PicClerk to Picture table\n");
	}
	pictureTable->releaseLock(name,x);
	//pictureLine->Release(name, 0);
#endif	
#ifdef PASSPORT
	//passportLine->Acquire(name, 0);
	passportTable->acquireLock(name,0);
	lineCheck(x);
	if ((passportTable->clerkCount() == 0)&& passNeedClerk);//&& !passportLine->nobody())
	{
		passportTable->addClerk(name,x,display);
		if(display)printf("[Manager] wakeup a PassClerk to Passport table\n");
	}
	passportTable->releaseLock(name,0);
	//passportLine->Release(name, 0);
#endif
#ifdef CASHIER
	//cashierLine->Acquire(name, 0);
	cashierTable->acquireLock(name,0);
	lineCheck(x);
	if ((cashierTable->clerkCount() == 0) && cashNeedClerk)
	{
		cashierTable->addClerk(name,x,display);
		if(display)printf("[Manager] wakeup a CashClerk to Cashier table\n");
	}
	cashierTable->releaseLock(name,0);
	//cashierLine->Release(name, 0);
#endif
*/
/*
	// check for number of customers in each line
	// add 1 clerk if # of customer in line is >3
	// must acquire the lock for both line and table before adding a clerk or checkin customer
	// must release all locks at completion.
#ifdef CHECK_LINE_3
	//applicationLine->Acquire(name, 0);
	applicationTable->acquireLock(name,0);
	if ((applicationLine->regCustomerCount() > 3) || (applicationLine->preferCustomerCount() > 3))
	{
		if(display)printf("[Manager]App Clerk %d ,Customer \n",applicationTable->clerkCount());
		applicationTable->addClerk(name,0);
		if(display)printf("[Manager] wakeup a AppClerk to Application table\n");
	}
	applicationTable->releaseLock(name,0);
	//applicationLine->Release(name, 0);

	//pictureLine->Acquire(name, 0);
	pictureTable->acquireLock(name,0);
	if ((pictureLine->regCustomerCount() > 3) || (pictureLine->preferCustomerCount() > 3))
	{
		pictureTable->addClerk(name,0);
		if(display)printf("[Manager]Pic Clerk %d Customer p%d r%d\n",pictureTable->clerkCount(),
				pictureLine->regCustomerCount(),pictureLine->preferCustomerCount());
		if(display)printf("[Manager] wakeup a PicClerk to Picture table\n");
	}
	pictureTable->releaseLock(name,0);
	//pictureLine->Release(name, 0);

	//passportLine->Acquire(name, 0);
	passportTable->acquireLock(name,0);
	if ((passportLine->regCustomerCount() > 3) || (passportLine->preferCustomerCount() > 3))
	{
		passportTable->addClerk(name,0);
		if(display)printf("[Manager] wakeup a PassClerk to Passport table\n");
	}
	passportTable->releaseLock(name,0);
	//passportLine->Release(name, 0);

	//cashierLine->Acquire(name, 0);
	cashierTable->acquireLock(name,0);
	if ((cashierLine->regCustomerCount() > 3) || (cashierLine->preferCustomerCount() > 3))
	{
		cashierTable->addClerk(name,0);
		if(display)printf("[Manager] wakeup a CashClerk to Cashier table\n");
	}
	cashierTable->releaseLock(name,0);
	//cashierLine->Release(name, 0);
	
#endif	
#ifdef SHOW_MONEY	
	// check for total amount of money currently collected at the office
	// Sum up all the money in each lines and all the money in each tables
	int officeMoney = applicationLine->reportMoney() + pictureLine->reportMoney() 
						+ passportLine->reportMoney() + cashierLine->reportMoney()
						+ applicationTable->reportMoney() + pictureTable->reportMoney()
						+ passportTable->reportMoney() + cashierTable->reportMoney();
	printf("[Manager] announce the office has collected total of %d dollars.........\n", officeMoney);
#endif	
	managerLock.Release();
}
void Problem2()
{
	office();
}
#endif
*/
