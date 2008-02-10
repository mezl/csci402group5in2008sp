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
	bool checkApplication();
	bool checkPicture();
	bool checkPassport();
	bool checkCashier();
	void chargeMoney(int mount);
	void punished();
	int getID(){return customerID;}
	char *getName(){return customerName;}
	void setClerk(Lock *l,Condition *c){
		setClerkLock->Acquire();
		clerkSet = true;
		clerkLock = l;
		clerkCondition = c;
		setClerkLock->Release();
	}
	bool getClerk(){
		setClerkLock->Acquire();
		if(!clerkSet)
			printf("[CUST]%s%d say:no clerk can help me\n",customerName,customerID);
		setClerkLock->Release();
		return clerkSet;
	
	}
	void wakeup(Lock *l){
		customerCondition->Signal(l);
	}
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
	bool clerkSet;
	Lock *setClerkLock;//given by free clerk
	Lock *clerkLock;//given by free clerk
	Condition *clerkCondition;//given by free clerk
	Condition *customerCondition;
};
#endif
