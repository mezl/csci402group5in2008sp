#ifndef OFFICE_H_DEFINED
#define OFFICE_H_DEFINED
#include "manager.cc"
#include "customer.h"
#include "clerk.h"
#include "cline.h"
#include "ctable.h"
#include "copyright.h"
#include "utility.h"
#include "timer.h"
#include "thread.h"
class Office
{
	public:
		Office(){
		
			applicationLine = new cLine(1);
			pictureLine = new cLine(2);
			passportLine = new cLine(3);
			cashierLine = new cLine(4);

			applicationTable = new cTable(1);
			pictureTable = new cTable(2);
			passportTable = new cTable(3);
			cashierTable= new cTable(4);
		}
		~Office(){}
		void run();

	private:
			void Manager(int x);
			void myCustomerForkFunc(int x);
			void myClerkForkFunc(int x);

			cLine *applicationLine ;
			cLine *pictureLine ;
			cLine *passportLine;
			cLine *cashierLine;

			cTable *applicationTable;
			cTable *pictureTable ;
			cTable *passportTable;
			cTable *cashierTable;

};


#endif	
