//customer.h

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "bool.h"
#include "copyright.h"
#include "system.h"
#include <time.h>
#include "synch.h"
#include "line.h"

class Customer
{
	private:
	int ID;
	int money;
	Line* applicationLine;
	Line* pictureLine;
	Line* passportLine;
	Line* cashierLine;
	bool applicationDone;
	bool pictureDone;
	bool passportDone;
	bool cashierDone;

	public:
	Customer(int ID_in, int money_in, Line* applicationLine_in, Line* pictureLine_in, Line* passportLine_in, Line* cashierLine_in);
	void customerRun();
	void gotoApplicationLine();
	void gotoPictureLine();
	void gotoPassportLine();
	void gotoCashierLine();
	void completeApplication();
	void completePicture();
	void completePassport();
	void completeCashier();
	int getID();
};
#endif
