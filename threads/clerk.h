#ifndef  CLERK_H_DEFINED
#define CLERK_H_DEFINED
#include "cline.h"
#include "table.h"
#include "customer.h"

class Clerk	
{
	public:
	Clerk(cLine *l);
	~Clerk();
	void run();
	virtual handleCustomer(Customer *c);
	private:
	cLine *cline;
	Table *table;
};
#endif


