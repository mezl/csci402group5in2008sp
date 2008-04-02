#include "syscall.h"
#include "string.h"


int main()
{
	int lock1 = CreateLock("AAA");
	int lock2 = CreateLock("BBB");
	int cond1 = CreateCondition("CCC");
	int i, z;

	println("[CLIENT 2]Main! start");
	println("[CLIENT 2]Start test");

	Acquire(lock1);
  	println("[CLIENT 2] signals on cond1 with lock1");
  	Signal(lock1,cond1);
	Release(lock1);
	
	Acquire(lock2);
	for(i=0; i<10000; i++)
	{
	}
	Release(lock2);
  	
	println("[CLIENT 2] job finished.. Exit");
}
