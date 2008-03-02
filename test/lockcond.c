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
  println("[func1] start");
  Acquire(lock1);
  println("[func1] signals on cond1 with lock1");
  Signal(lock1,cond1);

 /* Acquire(lock2);*/
  Release(lock1);
/* 
  println("func1 waits on cond3 with lock2");
  Wait(lock2,cond3);
  Release(lock2);


  Acquire(lock1);
  println("func1 signals on cond1 with lock1");
  Signal(lock1,cond1);
  Release(lock1);
*/
  println("[func1]func1 exits");
  
  Exit(0);
}

void func2(){

  println("[func2] start");
  Acquire(lock1);
  println("[func2] signals on cond2 with lock1");
  Signal(lock1,cond2);

/*  Acquire(lock2);*/
  Release(lock1);
/*
  println("func2 waits on cond3 with lock2\n");
  Wait(lock2,cond3);
  Release(lock2);
  
*/
  println("[func2] exits");
  Exit(0);
}

int main()
{

  println("[main]Main! start");
  println("[main]Start test");
  lock1 = CreateLock();
  lock2 = CreateLock();
  cond1 = CreateCondition();
  cond2 = CreateCondition();
/*  cond3 = CreateCondition();*/

  println("[main]Create All Lock & Cond");
  Acquire(lock1);
  println("[main]func1 forked by main");
  Fork(func1);
  println("[main]main waits on cond1 with lock1");
  Wait(lock1,cond1);
  println("[main]Main releaseing lock1");
  Release(lock1);

  println("[main]Main acquiring lock1");
  Acquire(lock1);
  println("[main]func2 will fork by main");
  Fork(func2);
  println("[main]func2 forked by main,now wait from func2");
  Wait(lock1,cond2);
  println("[main]main wake up on cond2 with lock1");
  println("[main]Main releaseing lock1");
  Release(lock1);
  println("[main]Main released lock1");

/*
  Acquire(lock2);
  println("main broadcasts on cond3 with lock2\n");
  Broadcast(lock2,cond3);


  Acquire(lock1);
  Release(lock2);

*/
  println("[main]main waits on cond1 with lock1\n");
/*  
  Wait(lock1,cond1);
  Release(lock1);
*/



  println("[main]main exits\n");


  DestroyLock(lock1);
  DestroyLock(lock2);
  DestroyCondition(cond1);
  DestroyCondition(cond2);
  DestroyCondition(cond3);
  Exit(0);
}


