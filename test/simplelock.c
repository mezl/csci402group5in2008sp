/* 
test lock and condition RPC
*/


#define NETWORK
#include "syscall.h"
#include "string.h"

void TestLockOnly()
{
	int lock1 = -1;
	int lock2 = -1;

	println("[CLIENT]Main! start");
	println("[CLIENT]Start test");
	lock1 = CreateLock("Peter");
	print("[CLIENT]lock 1 [name:Peter] id is ");
	printi(lock1);
	printn();

	lock2 = CreateLock("Max");
	print("[CLIENT]lock 2 [name:Max] id is ");
	printi(lock2);
	printn();
	println("[CLIENT]Start test");

	Acquire(lock1);
	Acquire(lock2);
	
	Release(lock1);
	Release(lock2);

	DestroyLock(lock1);
	DestroyLock(lock2);
	println("[CLIENT]main exits\n");
}

void TestAcquireLockFight()
{
	int lock1 = -1;
	int i, z;

	println("[CLIENT]Main! start");
	println("[CLIENT]Start test");
	lock1 = CreateLock("Peter");
	print("[CLIENT]lock 1 [name:Peter] id is ");
	printi(lock1);
	printn();

	Acquire(lock1);

	/* Client go busy for some time before releasing lock */
	for(i=0; i<10000; i++)
	{
		for(z=0; z<10; z++)
		{
		}
	}
	
	Release(lock1);
}


void TestLockAndCond()
{
	
}

int main()
{
	int mode = 0;
	/* mode == 0, run TestLockOnly() */
	/* mode == 1, run TestAcquireLockFight() */
	/* mode == 2, run TestLockAndCond() */
	
	
	if(mode == 0)
		TestLockOnly();
	else if(mode == 1)
		TestAcquireLockFight();
	else if(mode == 2)
		TestLockAndCond();

	Exit(0);
}


