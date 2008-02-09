//customer.h

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "bool.h"
#include "copyright.h"
#include "system.h"
#include <time.h>
#include "synch.h"
#include "cline.h"
class Customer
{

	public:
		Customer(){};		
		Customer(char *name,
			int ID_in, 
			int money_in, 
			cLine* applicationLine_in, 
			cLine* pictureLine_in, 
			cLine* passportLine_in, 
			cLine* cashierLine_in);
	void customerRun();
	void gotoApplicationLine();
	void gotoPictureLine();
	void gotoPassportLine();
	void gotoCashierLine();
	void completeApplication();
	void completePicture();
	void completePassport();
	void completeCashier();
	int getID(){return customerID;}
	char *getName(){return customerName;}

	private:
	int customerID;
	char *customerName;
	int money;
	cLine * applicationLine;
	cLine * pictureLine;
	cLine * passportLine;
	cLine * cashierLine;
	bool applicationDone;
	bool pictureDone;
	bool passportDone;
	bool cashierDone;
};
#endif
