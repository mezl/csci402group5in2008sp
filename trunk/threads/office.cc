// Office simulation code
// office.cc

#include "manager.cc"
#include "Customer.h"
#include "Clerk.h"
#include "Line.h"
#include "Table.h"
#include "copyright.h"
#include "utility.h"


Line *applicationLine = new Line(1);
Line *pictureLine = new Line(2);
Line *passportLine = new Line(3);
Line *cashierLine = new Line(4);

Table *applicationTable = new Table(1);
Table *pictureTable = new Table(2);
Table *passportLine = new Table(3);
Table *cashierLine = new Table(4);

void office()
{
	// create the manager
	Timer *timer = new Timer(Manager(), 0, true);

	// create 4 clerks (1 clerk for each table/job)


	// create 2 customers
	// customer 1 with ID = 1 and $1600
	Customer *cus1 = new Customer(1, 1600, &applicationLine, &pictureLine, &passportLine, &cashierLock);
	Thread *t1 = new Thread("Customer 1");
	t1 -> Fork(myCustomerForkFunc, (int t1));

	// customer 2 with ID = 2 and $1100
	Customer *cus2 = new Customer(2, 1100, &applicationLine, &pictureLine, &passportLine, &cashierLock);
	Thread *t2 = new Thread("Customer 2");
	t2 -> Fork(myCustomerForkFunc, (int t2));

}

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