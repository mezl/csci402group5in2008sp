#ifndef  CLERK_H_DEFINED
#define CLERK_H_DEFINED
#include "cline.h"
#include "table.h"
#include "Customer.h"

class Clerk	
{
	public:
	Clerk(Line *l);
	~Clerk();
	void run();
	virtual handleCustomer(Customer *c);
	private:
	Line *Line;
	Table *table;
};
#endif


