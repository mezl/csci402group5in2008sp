//customer.cc

#include "customer.h"
Customer::Customer(char *name,int ID_in, int money_in, cLine* applicationLine_in, cLine* pictureLine_in, cLine* passportLine_in, cLine* cashierLine_in)
{
	printf("Build Customer %d\n",ID_in);
	customerID = ID_in;
	customerName= name;
	money = money_in;
	applicationLine = applicationLine_in;
	pictureLine = pictureLine_in;
	passportLine = passportLine_in;
	cashierLine = cashierLine_in;
	applicationDone = false;
	pictureDone = false;
	passportDone = false;
	cashierDone = false;
	//random function initilization
	srand(time(NULL));
}

void Customer::customerRun()
{
	
	printf("Customer %d Start run \n",customerID);
	// Choose randomly between entering a application line or picture line
	// the customer will use the 500 dollars to get into the prefered line
	// whenever possible
	if ((rand()%2) == 0)
	{
		printf("Customer %d choose app first \n",customerID);
		gotoApplicationLine();
		printf("Customer %d go to pic line \n",customerID);
		gotoPictureLine();
	}
	else
	{
		printf("Customer %d go to pic line \n",customerID);
		gotoPictureLine();
		printf("Customer %d go to app line \n",customerID);
		gotoApplicationLine();
	}
	gotoPassportLine();
	gotoCashierLine();
}

void Customer::gotoApplicationLine()
{
	if(money > 500)
	{
		applicationLine -> preferAcquire(customerName,customerID);
		money = money-500;
		applicationLine -> addPreferLine(this, 500);
		applicationLine -> preferRelease(customerName,customerID);
	}
	else
	{
		applicationLine -> regAcquire(customerName,customerID);
		applicationLine -> addRegularLine(this);
		applicationLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoPictureLine()
{
	if(money > 500)
	{
		pictureLine -> preferAcquire(customerName,customerID);
		money = money-500;
//		pictureLine -> addPreferLine(this, 500);
		pictureLine -> preferRelease(customerName,customerID);
	}
	else
	{
		pictureLine -> regAcquire(customerName,customerID);
//		pictureLine -> addRegularLine(this);
		pictureLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoPassportLine()
{
	if(money > 500)
	{
		passportLine -> preferAcquire(customerName,customerID);
		money = money-500;
	//	passportLine -> addPreferLine(this, 500);
	//	passportLine -> preferRelease(customerName,customerID);	
	}
	else
	{
		passportLine -> regAcquire(customerName,customerID);
//		passportLine -> addRegularLine(this);
		passportLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoCashierLine()
{
	if(money > 500)
	{
		cashierLine -> preferAcquire(customerName,customerID);
		money = money-500;
//		cashierLine -> addPreferLine(this, 500);
		cashierLine -> preferRelease(customerName,customerID);
	}
	else
	{
		cashierLine -> regAcquire(customerName,customerID);
//		cashierLine -> addRegularLine(this);
		cashierLine -> regRelease(customerName,customerID);
	}
}

void Customer::completeApplication()
{
	applicationDone = true;
}

void Customer::completePicture()
{
	pictureDone = true;
}

void Customer::completePassport()
{
	passportDone = true;
}

void Customer::completeCashier()
{
	cashierDone = true;
}
