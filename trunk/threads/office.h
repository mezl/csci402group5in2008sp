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

cLine *applicationLine = new cLine(1);
cLine *pictureLine = new cLine(2);
cLine *passportLine = new cLine(3);
cLine *cashierLine = new cLine(4);

cTable *applicationTable = new cTable(1);
cTable *pictureTable = new cTable(2);
cTable *passportTable = new cTable(3);
cTable *cashierTable= new cTable(4);

void Manager(int x);
void myCustomerForkFunc(int x);
void myClerkForkFunc(int x);
void office();
void Manager(int x);

#endif	
