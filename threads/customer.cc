//customer.cc

#include "customer.h"
Customer::Customer(int ID_in, int money_in, cLine* applicationLine_in, cLine* pictureLine_in, cLine* passportLine_in, cLine* cashierLine_in)
{
	ID = ID_in;
	money = money_in;
	applicationLine = applicationLine_in;
	pictureLine = pictureLine_in;
	passportLine = passportLine_in;
	cashierLine = cashierLine_in;
	applicationDone = false;
	pictureDone = false;
	passportDone = false;
	cashierDone = false;
}

void Customer::customerRun()
{
	//random function initilization
	srand(time(NULL));

	// Choose randomly between entering a application line or picture line
	// the customer will use the 500 dollars to get into the prefered line
	// whenever possible
	if ((rand()%2) == 0)
	{
		gotoApplicationLine();
		gotoPictureLine();
	}
	else
	{
		gotoPictureLine();
		gotoApplicationLine();
	}
	gotoPassportLine();
	gotoCashierLine();
}

void Customer::gotoApplicationLine()
{
	if(money > 500)
	{
		applicationLine -> preferAcquire();
		money = money-500;
		applicationLine -> addPreferLine(this, 500);
		applicationLine -> preferRelease();
	}
	else
	{
		applicationLine -> regAcquire();
		applicationLine -> addRegularLine(this);
		applicationLine -> regRelease();
	}
}

void Customer::gotoPictureLine()
{
	if(money > 500)
	{
		pictureLine -> preferAcquire();
		money = money-500;
		pictureLine -> addPreferLine(this, 500);
		pictureLine -> preferRelease();
	}
	else
	{
		pictureLine -> regAcquire();
		pictureLine -> addRegularLine(this);
		pictureLine -> regRelease();
	}
}

void Customer::gotoPassportLine()
{
	if(money > 500)
	{
		passportLine -> preferAcquire();
		money = money-500;
		passportLine -> addPreferLine(this, 500);
		passportLine -> preferRelease();	
	}
	else
	{
		passportLine -> regAcquire();
		passportLine -> addRegularLine(this);
		passportLine -> regRelease();
	}
}

void Customer::gotoCashierLine()
{
	if(money > 500)
	{
		cashierLine -> preferAcquire();
		money = money-500;
		cashierLine -> addPreferLine(this, 500);
		cashierLine -> preferRelease();
	}
	else
	{
		cashierLine -> regAcquire();
		cashierLine -> addRegularLine(this);
		cashierLine -> regRelease();
	}
}

int Customer::getID()
{
	return ID;
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
