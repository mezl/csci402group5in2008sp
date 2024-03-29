//	Simple test cases for the threads assignment.
//

#include "copyright.h"
#include "system.h"
#ifdef CHANGED
#include "synch.h"
#endif
#ifdef CHANGED
// --------------------------------------------------
// Test Suite
// --------------------------------------------------


// --------------------------------------------------
// Test 1 - see TestSuite() for details
// --------------------------------------------------
Semaphore t1_s1("t1_s1",0);       // To make sure t1_t1 acquires the
                                  // lock before t1_t2
Semaphore t1_s2("t1_s2",0);       // To make sure t1_t2 Is waiting on the 
                                  // lock before t1_t3 releases it
Semaphore t1_s3("t1_s3",0);       // To make sure t1_t1 does not release the
                                  // lock before t1_t3 tries to acquire it
Semaphore t1_done("t1_done",0);   // So that TestSuite knows when Test 1 is
                                  // done
Lock t1_l1("t1_l1");		  // the lock tested in Test 1

// --------------------------------------------------
// t1_t1() -- test1 thread 1
//     This is the rightful lock owner
// --------------------------------------------------
void t1_t1() {
    t1_l1.Acquire();
    t1_s1.V();  // Allow t1_t2 to try to Acquire Lock
 
    printf ("%s: Acquired Lock %s, waiting for t3\n",currentThread->getName(),
	    t1_l1.getName());
    t1_s3.P();
    printf ("%s: working in CS\n",currentThread->getName());
    for (int i = 0; i < 1000000; i++) ;
    printf ("%s: Releasing Lock %s\n",currentThread->getName(),
	    t1_l1.getName());
    t1_l1.Release();
    t1_done.V();
}

// --------------------------------------------------
// t1_t2() -- test1 thread 2
//     This thread will wait on the held lock.
// --------------------------------------------------
void t1_t2() {

    t1_s1.P();	// Wait until t1 has the lock
    t1_s2.V();  // Let t3 try to acquire the lock

    printf("%s: trying to acquire lock %s\n",currentThread->getName(),
	    t1_l1.getName());
    t1_l1.Acquire();

    printf ("%s: Acquired Lock %s, working in CS\n",currentThread->getName(),
	    t1_l1.getName());
    for (int i = 0; i < 10; i++)
	;
    printf ("%s: Releasing Lock %s\n",currentThread->getName(),
	    t1_l1.getName());
    t1_l1.Release();
    t1_done.V();
}

// --------------------------------------------------
// t1_t3() -- test1 thread 3
//     This thread will try to release the lock illegally
// --------------------------------------------------
void t1_t3() {

    t1_s2.P();	// Wait until t2 is ready to try to acquire the lock

    t1_s3.V();	// Let t1 do it's stuff
    for ( int i = 0; i < 3; i++ ) {
	printf("%s: Trying to release Lock %s\n",currentThread->getName(),
	       t1_l1.getName());
	t1_l1.Release();
    }
}

// --------------------------------------------------
// Test 2 - see TestSuite() for details
// --------------------------------------------------
Lock t2_l1("t2_l1");		// For mutual exclusion
Condition t2_c1("t2_c1");	// The condition variable to test
Semaphore t2_s1("t2_s1",0);	// To ensure the Signal comes before the wait
Semaphore t2_done("t2_done",0);     // So that TestSuite knows when Test 2 is
                                  // done

// --------------------------------------------------
// t2_t1() -- test 2 thread 1
//     This thread will signal a variable with nothing waiting
// --------------------------------------------------
void t2_t1() {
    t2_l1.Acquire();
    printf("%s: Lock %s acquired, signalling %s\n",currentThread->getName(),
	   t2_l1.getName(), t2_c1.getName());
    t2_c1.Signal(&t2_l1);
    printf("%s: Releasing Lock %s\n",currentThread->getName(),
	   t2_l1.getName());
    t2_l1.Release();
    t2_s1.V();	// release t2_t2
    t2_done.V();
}

// --------------------------------------------------
// t2_t2() -- test 2 thread 2
//     This thread will wait on a pre-signalled variable
// --------------------------------------------------
void t2_t2() {
    t2_s1.P();	// Wait for t2_t1 to be done with the lock
    t2_l1.Acquire();
    printf("%s: Lock %s acquired, waiting on %s\n",currentThread->getName(),
	   t2_l1.getName(), t2_c1.getName());
    t2_c1.Wait(&t2_l1);
    printf("%s: Releasing Lock %s\n",currentThread->getName(),
	   t2_l1.getName());
    t2_l1.Release();
}
// --------------------------------------------------
// Test 3 - see TestSuite() for details
// --------------------------------------------------
Lock t3_l1("t3_l1");		// For mutual exclusion
Condition t3_c1("t3_c1");	// The condition variable to test
Semaphore t3_s1("t3_s1",0);	// To ensure the Signal comes before the wait
Semaphore t3_done("t3_done",0); // So that TestSuite knows when Test 3 is
                                // done

// --------------------------------------------------
// t3_waiter()
//     These threads will wait on the t3_c1 condition variable.  Only
//     one t3_waiter will be released
// --------------------------------------------------
void t3_waiter() {
    t3_l1.Acquire();
    t3_s1.V();		// Let the signaller know we're ready to wait
    printf("%s: Lock %s acquired, waiting on %s\n",currentThread->getName(),
	   t3_l1.getName(), t3_c1.getName());
    t3_c1.Wait(&t3_l1);
    printf("%s: freed from %s\n",currentThread->getName(), t3_c1.getName());
    t3_l1.Release();
    t3_done.V();
}


// --------------------------------------------------
// t3_signaller()
//     This threads will signal the t3_c1 condition variable.  Only
//     one t3_signaller will be released
// --------------------------------------------------
void t3_signaller() {

    // Don't signal until someone's waiting
    
    for ( int i = 0; i < 5 ; i++ ) 
	t3_s1.P();
    t3_l1.Acquire();
    printf("%s: Lock %s acquired, signalling %s\n",currentThread->getName(),
	   t3_l1.getName(), t3_c1.getName());
    t3_c1.Signal(&t3_l1);
    printf("%s: Releasing %s\n",currentThread->getName(), t3_l1.getName());
    t3_l1.Release();
    t3_done.V();
}
 
// --------------------------------------------------
// Test 4 - see TestSuite() for details
// --------------------------------------------------
Lock t4_l1("t4_l1");		// For mutual exclusion
Condition t4_c1("t4_c1");	// The condition variable to test
Semaphore t4_s1("t4_s1",0);	// To ensure the Signal comes before the wait
Semaphore t4_done("t4_done",0); // So that TestSuite knows when Test 4 is
                                // done

// --------------------------------------------------
// t4_waiter()
//     These threads will wait on the t4_c1 condition variable.  All
//     t4_waiters will be released
// --------------------------------------------------
void t4_waiter() {
    t4_l1.Acquire();
    t4_s1.V();		// Let the signaller know we're ready to wait
    printf("%s: Lock %s acquired, waiting on %s\n",currentThread->getName(),
	   t4_l1.getName(), t4_c1.getName());
    t4_c1.Wait(&t4_l1);
    printf("%s: freed from %s\n",currentThread->getName(), t4_c1.getName());
    t4_l1.Release();
    t4_done.V();
}


// --------------------------------------------------
// t2_signaller()
//     This thread will broadcast to the t4_c1 condition variable.
//     All t4_waiters will be released
// --------------------------------------------------
void t4_signaller() {

    // Don't broadcast until someone's waiting
    
    for ( int i = 0; i < 5 ; i++ ) 
	t4_s1.P();
    t4_l1.Acquire();
    printf("%s: Lock %s acquired, broadcasting %s\n",currentThread->getName(),
	   t4_l1.getName(), t4_c1.getName());
    t4_c1.Broadcast(&t4_l1);
    printf("%s: Releasing %s\n",currentThread->getName(), t4_l1.getName());
    t4_l1.Release();
    t4_done.V();
}
// --------------------------------------------------
// Test 5 - see TestSuite() for details
// --------------------------------------------------
Lock t5_l1("t5_l1");		// For mutual exclusion
Lock t5_l2("t5_l2");		// Second lock for the bad behavior
Condition t5_c1("t5_c1");	// The condition variable to test
Semaphore t5_s1("t5_s1",0);	// To make sure t5_t2 acquires the lock after
                                // t5_t1

// --------------------------------------------------
// t5_t1() -- test 5 thread 1
//     This thread will wait on a condition under t5_l1
// --------------------------------------------------
void t5_t1() {
    t5_l1.Acquire();
    t5_s1.V();	// release t5_t2
    printf("%s: Lock %s acquired, waiting on %s\n",currentThread->getName(),
	   t5_l1.getName(), t5_c1.getName());
    t5_c1.Wait(&t5_l1);
    printf("%s: Releasing Lock %s\n",currentThread->getName(),
	   t5_l1.getName());
    t5_l1.Release();
}

// --------------------------------------------------
// t5_t1() -- test 5 thread 1
//     This thread will wait on a t5_c1 condition under t5_l2, which is
//     a Fatal error
// --------------------------------------------------
void t5_t2() {
    t5_s1.P();	// Wait for t5_t1 to get into the monitor
    t5_l1.Acquire();
    t5_l2.Acquire();
    printf("%s: Lock %s acquired, signalling %s\n",currentThread->getName(),
	   t5_l2.getName(), t5_c1.getName());
    t5_c1.Signal(&t5_l2);
    printf("%s: Releasing Lock %s\n",currentThread->getName(),
	   t5_l2.getName());
    t5_l2.Release();
    printf("%s: Releasing Lock %s\n",currentThread->getName(),
	   t5_l1.getName());
    t5_l1.Release();
}

// --------------------------------------------------
// TestSuite()
//     This is the main thread of the test suite.  It runs the
//     following tests:
//
//       1.  Show that a thread trying to release a lock it does not
//       hold does not work
//
//       2.  Show that Signals are not stored -- a Signal with no
//       thread waiting is ignored
//
//       3.  Show that Signal only wakes 1 thread
//
//	 4.  Show that Broadcast wakes all waiting threads
//
//       5.  Show that Signalling a thread waiting under one lock
//       while holding another is a Fatal error
//
//     Fatal errors terminate the thread in question.
// --------------------------------------------------
void TestSuite() {
    Thread *t;
    char *name;
    int i;
    
    // Test 1

    printf("Starting Test 1\n");

    t = new Thread("t1_t1");
    t->Fork((VoidFunctionPtr)t1_t1,0);

    t = new Thread("t1_t2");
    t->Fork((VoidFunctionPtr)t1_t2,0);

    t = new Thread("t1_t3");
    t->Fork((VoidFunctionPtr)t1_t3,0);

    // Wait for Test 1 to complete
    for (  i = 0; i < 2; i++ )
	t1_done.P();

    // Test 2

    printf("Starting Test 2.  Note that it is an error if thread t2_t2\n");
    printf("completes\n");

    t = new Thread("t2_t1");
    t->Fork((VoidFunctionPtr)t2_t1,0);

    t = new Thread("t2_t2");
    t->Fork((VoidFunctionPtr)t2_t2,0);

    // Wait for Test 2 to complete
    t2_done.P();

    // Test 3

    printf("Starting Test 3\n");

    for (  i = 0 ; i < 5 ; i++ ) {
	name = new char [20];
	sprintf(name,"t3_waiter%d",i);
	t = new Thread(name);
	t->Fork((VoidFunctionPtr)t3_waiter,0);
    }
    t = new Thread("t3_signaller");
    t->Fork((VoidFunctionPtr)t3_signaller,0);

    // Wait for Test 3 to complete
    for (  i = 0; i < 2; i++ )
	t3_done.P();

    // Test 4

    printf("Starting Test 4\n");

    for (  i = 0 ; i < 5 ; i++ ) {
	name = new char [20];
	sprintf(name,"t4_waiter%d",i);
	t = new Thread(name);
	t->Fork((VoidFunctionPtr)t4_waiter,0);
    }
    t = new Thread("t4_signaller");
    t->Fork((VoidFunctionPtr)t4_signaller,0);

    // Wait for Test 4 to complete
    for (  i = 0; i < 6; i++ )
	t4_done.P();

    // Test 5

    printf("Starting Test 5.  Note that it is an error if thread t5_t1\n");
    printf("completes\n");

    t = new Thread("t5_t1");
    t->Fork((VoidFunctionPtr)t5_t1,0);

    t = new Thread("t5_t2");
    t->Fork((VoidFunctionPtr)t5_t2,0);

}
///////////////////////////////////////////////////////////////////////////////
// Part 2
//
//Enter function are Office
//use the define below can turn on/off code
//
//
///////////////////////////////////////////////////////////////////////////////
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

//Set how many customer and clerk want to run
#define CUSTOMER_NUM 49
#define CLERK_NUM 59
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
