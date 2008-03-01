/* 
test for lock and condition system calls 
main shall first fork func1 and waits to be signaled by it. 
main then forks func2 and waits to be signaled by it.
func1 and func2 both wait to be broadcasted by main before they exist

the write syscalls to the console print out MSG that describe the desired 
synchonization described above. 
*/


#include "syscall.h"
#include "string.h"
  int lock1; 
  int lock2; 
  int cond1;
  int cond2;
  int cond3;


void func1(){
  print("func 1 start");
  Acquire(lock1);
  println("func1 signals on cond1 with lock1\n");
  Signal(lock1,cond1);

  Acquire(lock2);
  Release(lock1);
 
  println("func1 waits on cond3 with lock2\n");
  Wait(lock2,cond3);
  Release(lock2);


  Acquire(lock1);
  println("func1 signals on cond1 with lock1\n");
  Signal(lock1,cond1);
  Release(lock1);

  println("func1 exits\n");
  Exit(0);
}

void func2(){

  Acquire(lock1);
  println("func2 signals on cond2 with lock1\n");
  Signal(lock1,cond2);

  Acquire(lock2);
  Release(lock1);

  println("func2 waits on cond3 with lock2\n");
  Wait(lock2,cond3);
  Release(lock2);
  

  println("func2 exits\n");
  Exit(0);
}

int main()
{

  print("Main!  start");
  Write("Start test\n",11,ConsoleOutput);
  lock1 = CreateLock();
  lock2 = CreateLock();
  cond1 = CreateCondition();
  cond2 = CreateCondition();
  cond3 = CreateCondition();

  Write("Create All Lock & Cond\n",23,ConsoleOutput);
  Acquire(lock1);
  Write("func1 forked by main\n",21,ConsoleOutput);
  Fork(func1);
  Write("main waits on cond1 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond1);
  Release(lock1);


  Acquire(lock1);
  Write("func2 forked by main\n",21,ConsoleOutput);
  Fork(func2);
  Write("main waits on cond2 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond2);
  Release(lock1);


  Acquire(lock2);
  Write("main broadcasts on cond3 with lock2\n",36,ConsoleOutput);
  Broadcast(lock2,cond3);


  Acquire(lock1);
  Release(lock2);


  Write("main waits on cond1 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond1);
  Release(lock1);



  Write("main exits\n",12,ConsoleOutput);

  DestroyLock(lock1);
  DestroyLock(lock2);
  DestroyCondition(cond1);
  DestroyCondition(cond2);
  DestroyCondition(cond3);
  Exit(0);
}


