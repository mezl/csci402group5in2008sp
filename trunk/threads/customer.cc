//customer.cc

#include "customer.h"
Customer::Customer(char *name,int ID_in, int money_in, cLine* applicationLine_in, cLine* pictureLine_in, cLine* passportLine_in, cLine* cashierLine_in)
{
	printf("[CUST]Build Customer %d\n",ID_in);
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
	
	printf("[CUST]Customer %d Start run \n",customerID);
	// Choose randomly between entering a application line or picture line
	// the customer will use the 500 dollars to get into the prefered line
	// whenever possible
	if ((rand()%2) == 0)
	{
		printf("[CUST]Customer %d choose app first \n",customerID);
		gotoApplicationLine();
		printf("[CUST]Customer %d then go to pic line \n",customerID);
		gotoPictureLine();
	}
	else
	{
		printf("[CUST]Customer %d choose pic first \n",customerID);
		gotoPictureLine();
		printf("[CUST]Customer %d then go to app line \n",customerID);
		gotoApplicationLine();
	}
	printf("[CUST]Customer %d finish both app & pic \n",customerID);
	gotoPassportLine();
	gotoCashierLine();
}

void Customer::gotoApplicationLine()
{
	if(money > 500)
	{
		printf("[CUST]Customer %d go to prefer app line \n",customerID);
		applicationLine -> preferAcquire(customerName,customerID);
		printf("[CUST]Customer %d get prefer app line lock\n",customerID);
		money = money-500;
		applicationLine -> addPreferLine((int)this, 500);
		printf("[CUST]Customer %d wake up in %s%d \n",customerID,
				applicationLine->getName(),
				applicationLine->getID());
		applicationLine -> preferRelease(customerName,customerID);
	}
	else
	{
		printf("[CUST]Customer %d go to reg app line \n",customerID);
		applicationLine -> regAcquire(customerName,customerID);
		applicationLine -> addRegLine((int)this);
		applicationLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoPictureLine()
{
	if(money > 500)
	{
		printf("[CUST]Customer %d go to prefer pic line \n",customerID);
		pictureLine -> preferAcquire(customerName,customerID);
		money = money-500;
		pictureLine -> addPreferLine((int)this, 500);
		pictureLine -> preferRelease(customerName,customerID);
	}
	else
	{
		printf("[CUST]Customer %d go to reg pic line \n",customerID);
		pictureLine -> regAcquire(customerName,customerID);
		pictureLine -> addRegLine((int)this);
		pictureLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoPassportLine()
{
	if(money > 500)
	{
		printf("[CUST]Customer %d go to prefer passport line \n",customerID);
		passportLine -> preferAcquire(customerName,customerID);
		money = money-500;
		passportLine -> addPreferLine((int)this, 500);
		passportLine -> preferRelease(customerName,customerID);	
	}
	else
	{
		printf("[CUST]Customer %d go to reg passport line \n",customerID);
		passportLine -> regAcquire(customerName,customerID);
		passportLine -> addRegLine((int)this);
		passportLine -> regRelease(customerName,customerID);
	}
}

void Customer::gotoCashierLine()
{
	if(money > 500)
	{
		printf("[CUST]Customer %d go to prefer cashier line \n",customerID);
		cashierLine -> preferAcquire(customerName,customerID);
		money = money-500;
		cashierLine -> addPreferLine((int)this, 500);
		cashierLine -> preferRelease(customerName,customerID);
	}
	else
	{
		printf("[CUST]Customer %d go to reg cashier line \n",customerID);
		cashierLine -> regAcquire(customerName,customerID);
		cashierLine -> addRegLine((int)this);
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

bool Customer::checkApplication()
{
	return applicationDone;
}

bool Customer::checkPicture()
{
	return pictureDone;
}

bool Customer::checkPassport()
{
	return passportDone;
}

bool Customer::checkCashier()
{
	return cashierDone;
}

void Customer::chargeMoney(int mount)
{
	money -= mount;
}

void Customer::punished()
{
	currentThread -> Yield();
}
