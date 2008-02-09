// Office simulation code
// office.cc

#include "manager.cc"
#include "customer.h"
#include "clerk.h"
#include "cline.h"
#include "table.h"
#include "copyright.h"
#include "utility.h"


cLine *applicationLine = new cLine(1);
cLine *pictureLine = new cLine(2);
cLine *passportLine = new cLine(3);
cLine *cashierLine = new cLine(4);

Table *applicationTable = new Table(1);
Table *pictureTable = new Table(2);
Table *passportTable = new Table(3);
Table *cashierTable= new Table(4);

void Manager(int x);

void myCustomerForkFunc(int x)
{
	Customer *c = (Customer*)x;
	c -> customerRun();

	delete c;
}

void myClerkForkFunc(int x)
{
	Clerk *cl = (Clerk*)x;
	cl -> run();

	delete cl;
}
void office()
{
	// create the manager
	Timer *timer = new Timer(Manager, 0, false);

	// create 4 clerks (1 clerk for each table/job)


	// create 2 customers
	// customer 1 with ID = 1 and $1600
	Customer *cus1 = new Customer(1, 1600, applicationLine, pictureLine, passportLine, cashierLine);
	Thread *t1 = new Thread("Customer 1");
	t1 -> Fork(myCustomerForkFunc, (int)t1);

	// customer 2 with ID = 2 and $1100
	Customer *cus2 = new Customer(2, 1100, applicationLine, pictureLine, passportLine, cashierLine);
	Thread *t2 = new Thread("Customer 2");
	t2 -> Fork(myCustomerForkFunc, (int) t2);

}

void Manager(int x)
{
	// check each table for number of clerks
	// add 1 clerk if table is empty

	applicationTable->acquireLock();
	if (applicationTable->clerkCount() == 0)
	{
		applicationTable->addClerk();
	}
	applicationTable->releaseLock();

	pictureTable->acquireLock();
	if (pictureTable->clerkCount() == 0)
	{
		pictureTable->addClerk();
	}
	pictureTable->releaseLock();

	passportTable->acquireLock();
	if (passportTable->clerkCount() == 0)
	{
		passportTable->addClerk();
	}
	passportTable->releaseLock();
	
	cashierTable->acquireLock();
	if (cashierTable->clerkCount() == 0)
	{
		cashierTable->addClerk();
	}
	cashierTable->releaseLock();


	// check for number of customers in each line
	// add 1 clerk if # of customer in line is >3
	// must acquire the lock for both line and table before adding a clerk or checkin customer
	// must release all locks at completion.

	applicationLine->regAcquire();
	applicationLine->preferAcquire();
	applicationTable->acquireLock();
	if (applicationLine->customerCount() > 3)
	{
		applicationTable->addClerk();
	}
	applicationLine->regRelease();
	applicationLine->preferRelease();
	applicaitonTable->releaseLock();

	pictureline->regAcquire();
	pictureLine->preferAcquire();
	pictureTable->acquireLock();
	if (pictureLine->customerCount() > 3)
	{
		pictureTable->addClerk();
	}
	pictureLine->regRelease();
	pictureLine->preferRelease();
	pictureTable->releaseLock();

	passportline->regAcquire();
	passportLine->preferAcquire();
	passportTable->acquireLock();
	if (passportLine->customerCount() > 3)
	{
		passportTable->addClerk();
	}
	passportLine->regRelease();
	passportLine->preferRelease();
	passportTable->releaseLock();

	cashierline->regAcquire();
	cashierLine->preferAcquire();
	cashierTable->acquireLock();
	if (cashierLine->customerCount() > 3)
	{
		cashierTable->addClerk();
	}
	cashierLine->regRelease();
	cashierLine->preferRelease();
	cashierTable->releaseLock();
}
