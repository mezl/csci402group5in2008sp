#include "syscall.h"

void output(char *msg)
{
    Write( msg, strlen(msg), ConsoleOutput );	
}
