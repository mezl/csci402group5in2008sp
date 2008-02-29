/*

#ifndef _STRING_H
#define _STRING_H

*/


#include "syscall.h"

inline int itsStrlen(char *s) {
    int len = 0;

    while(*s++ != '\0' ) len++;
    return len;
}

inline void itsStrcpy(char *to, char *from) {

    while (*to++ = *from++)
	;
}

inline char *itoa(int z, int base) {

    char a[10];
    static char b[10];
    int l=0;
    int i;
    int y;
    unsigned int x = (unsigned int) z;

    if ( x == 0 ) {
	itsStrcpy(b,"0");
	return b;
    }
    while (x && l < 9) {
	y = x % base;
	if ( y < 10 ) 
	    a[l++] = '0' + y;
	else
	    a[l++] = 'a' + (y-10);
	x /= base;
    }
    for ( i = 0; i < l; i++ )
	b[i] = a[l-i-1];
    b[l] = 0;
    return b;
}

inline void print(char *s) {
    Write(s,itsStrlen(s),ConsoleOutput);
}

inline void println(char *s) {
    Write(s,itsStrlen(s),ConsoleOutput);
    Write("\n",1,ConsoleOutput);
}
inline void printi(int i) {
    char *s = itoa(i,10);
    Write(s,itsStrlen(s),ConsoleOutput);
}
inline void printx(int i) {
    char *s = itoa(i,16);
    Write(s,itsStrlen(s),ConsoleOutput);
}
inline printn() {
    Write("\n",1,ConsoleOutput);
}

/*
#endif
*/
