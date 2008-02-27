/* test for lock and condition system calls 
basic structure is the same as lockcond.c:

main shall first fork func1 and waits to be signaled by it. 
main then forks func2 and waits to be signaled by it.
func1 and func2 both wait to be broadcasted by main before they exist

the test intentionally tries to crash nachos by misusing 
lock and condition variable syscalls

the write syscalls to the console print out MSG that describe the test 
*/


#include "syscall.h"

void func1();
void func2();


  int lock1; 
  int lock2; 
  int cond1;
  int cond2;
  int cond3;

  int lockx;
  int condx;

int main()
{

  lock1 = CreateLock();
  lock2 = CreateLock();
  cond1 = CreateCondition();
  cond2 = CreateCondition();
  cond3 = CreateCondition();

  lockx = 888;
  condx = 999;


  /************ premitive lock tests **************/

  /* destroying a non-exist lock */
  DestroyLock(lockx);

  /* Release should fail. should trigger msg in lock class because 
     this is a lock that hasn't been acquired */
  Release(lock1);
  /* should trigger mag in syscall function because 
     this is a invalid lock ID */
  Acquire(150);
  /* lock should not be released because the previous Acquire should fail */
  Release(150);


  /************ premitive condition test **************/

  /* destroying a non-exist condition */
  DestroyCondition(condx);
  


  Acquire(lock1);
  Write("func1 forked by main\n",21,ConsoleOutput);
  Fork(func1);


  /************* tests for Wait() **************/

  /* this wait should not work because lockx does not exist */ 
  Write("main waits on cond1 with lockx\n",31,ConsoleOutput);
  Wait(lockx,cond1);

  /* this wait should not work because condx does not exist */ 
  Write("main waits on condx with lock1\n",31,ConsoleOutput);
  Wait(lock1,condx);

  Write("main waits on cond1 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond1);
  Release(lock1);


  Acquire(lock1);
  Write("func2 forked by main\n",21,ConsoleOutput);
  Fork(func2);
  Write("main waits on cond2 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond2);


  /************* tests for Broadcast() **************/

  Release(lock1);


  Acquire(lock2);

  /* broadcast should fail because lockx does not exist */
  Write("main broadcasts on cond3 with lockx\n",36,ConsoleOutput);
  Broadcast(lockx,cond3);

  /* broadcast should fail because condx does not exist */
  Write("main broadcasts on cond3 with lockx\n",36,ConsoleOutput);
  Broadcast(lock2,condx);

  Write("main broadcasts on cond3 with lock2\n",36,ConsoleOutput);
  Broadcast(lock2,cond3);


  Acquire(lock1);
  Release(lock2);


  Write("main waits on cond1 with lock1\n",31,ConsoleOutput);
  Wait(lock1,cond1);
  Release(lock1);



  Write("main exists\n",12,ConsoleOutput);

  DestroyLock(lock1);
  DestroyLock(lock2);
  DestroyCondition(cond1);
  DestroyCondition(cond2);
  DestroyCondition(cond3);
  Exit(0);
}

void func1(){

  Acquire(lock2);

  /************* tests for Signal() **************/

  /* this signal should fail because using the wrong lock*/
  Write("func1 signals on cond1 with lock2\n",34,ConsoleOutput);
  Signal(lock2,cond1);

  /* this signal should fail because lockx deos not exist */
  Write("func1 signals on condx with lock2\n",34,ConsoleOutput);
  Signal(lockx,cond1);

  /* this signal should fail because condx deos not exist */
  Write("func1 signals on condx with lock2\n",34,ConsoleOutput);
  Signal(lock2,condx);
  Release(lock2);




  Acquire(lock1);
  Write("func1 signals on cond1 with lock1\n",34,ConsoleOutput);
  Signal(lock1,cond1);

  Acquire(lock2);
  Release(lock1);
 
  Write("func1 waits on cond3 with lock2\n",32,ConsoleOutput);
  Wait(lock2,cond3);
  Release(lock2);


  Acquire(lock1);
  Write("func1 signals on cond1 with lock1\n",34,ConsoleOutput);
  Signal(lock1,cond1);
  Release(lock1);


  Write("func1 exists\n",13,ConsoleOutput);
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
  Release(lock2);

  Write("func2 exists\n",13,ConsoleOutput);
  Exit(0);
}

