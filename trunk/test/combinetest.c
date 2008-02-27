/* test combining all syscalls
 
main shall first fork func1 and waits to be signaled by it. 
main then forks func2 and waits to be signaled by it.
func1 and func2 both wait to be broadcasted by main before they exist

when any thread wakes up it calls op2.

the write syscalls to the console print out MSG that describe the desired 
synchonization described above.
*/

#include "syscall.h"


void func1();
void func2();

  int lock1; 
  int lock2; 
  int cond1;
  int cond2;
  int cond3;

int main()
{

  lock1 = CreateLock();
  lock2 = CreateLock();
  cond1 = CreateCondition();
  cond2 = CreateCondition();
  cond3 = CreateCondition();

  Acquire(lock1);
  Write("func1 forked by main\n",21,ConsoleOutput);
  Fork(func1);
  Write("main waits on cond1 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond1);

  Exec("../test/op2");
  Write("op2 executed by combinetest after first wake-up\n",48,ConsoleOutput);

  Release(lock1);



  Yield();



  Acquire(lock1);
  Write("func2 forked by main\n",21,ConsoleOutput);
  Fork(func2);
  Write("main waits on cond2 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond2);

  Exec("../test/op2");
  Write("op2 executed by combinetest after second wake-up\n",49,ConsoleOutput);

  Release(lock1);



  Yield();



  Acquire(lock2);
  Write("main broadcasts on cond3 with lock2\n",36,ConsoleOutput);
  Broadcast(lock2,cond3);
  Release(lock2);

  Write("main exists\n",12,ConsoleOutput);
  Exit(0);
}

void func1(){

  Acquire(lock1);
  Write("func1 signals on cond1 with lock1\n",34,ConsoleOutput);
  Signal(lock1,cond1);

  Acquire(lock2);
  Release(lock1);
 
  Write("func1 waits on cond3 with lock2\n",32,ConsoleOutput);
  Wait(lock2,cond3);

  Exec("../test/op2");
  Write("op2 executed by func1 after being broadcasted\n",46,ConsoleOutput);

  Release(lock2);

  Write("func1 exists\n",13,ConsoleOutput);

  DestroyLock(lock1);
  DestroyLock(lock2);
  DestroyCondition(cond1);
  DestroyCondition(cond2);
  DestroyCondition(cond3);
  Exit(0);
}

void func2(){

  Acquire(lock1);
  Write("func2 signals on cond2 with lock1\n",34,ConsoleOutput);
  Signal(lock1,cond2);

  Acquire(lock2);
  Release(lock1);

  Write("func2 waits on cond3 with lock2\n",32,ConsoleOutput);
  Wait(lock2,cond3);

  Exec("../test/op2");
  Write("op2 executed by func2 after being broadcasted\n",46,ConsoleOutput);

  Release(lock2);

  Write("func2 exists\n",13,ConsoleOutput);
  Exit(0);
}



