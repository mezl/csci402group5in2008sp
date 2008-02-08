#ifdef  CLERK_H_DEFINED
#define CLERK_H_DEFINED


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


