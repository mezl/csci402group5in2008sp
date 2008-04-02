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
inline void printx(int i) {
    char *s = itoa(i,16);
    Write(s,itsStrlen(s),ConsoleOutput);
}
inline void printi(int i) {
    char *s = itoa(i,10);
    Write(s,itsStrlen(s),ConsoleOutput);
}
inline void printn() {
    Write("\n",1,ConsoleOutput);
}
/*
#endif
*/
typedef char * va_list; 
#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int)-1))
#define va_start(ap,v) ( ap=(va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap) (ap = (va_list)0 ) 
/*======================================================================*
                                vsprintf
 *======================================================================*/
inline int vsprintf(char *buf, const char *fmt, va_list args)
{
    char*   p;
    char*   q;
    char*   r;
    char    tmp[256];
    int len,i;
    
    va_list p_arg = args;
    for (p=buf;*fmt;fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }
        fmt++;
        switch (*fmt) {
        case 'd':
            q= itoa(*(int*)p_arg,10);
            itsStrcpy(p, q);
            p_arg += 4;
            p += itsStrlen(q);
            break;
        case 's':
        /*r = va_arg(p_arg, char *);
         len = itsStrlen(r);        */
       /*  while(*r != '\0'){*/
            /*p++ = *r++;*/
         /*}*/
         /*/p_arg += len;*/
         
        /*
            r= (char*)p_arg;
            itsStrcpy(p,r);
            p_arg += 1;
            p +=itsStrlen(r);
            */
            break;
        case 'c':
            q = (char*)p_arg;
            itsStrcpy(p, q);
            p_arg += 1;
            p += itsStrlen(q);
            break;
        case 'x':
            q= itoa(*(int*)p_arg,16);
            itsStrcpy(p, q);
            p_arg += 4;
            p += itsStrlen(q);
            break;
        default:
            break;
        }
    }

    return (p - buf);
}

/*======================================================================*
                                 printf
{*======================================================================*/
int printf(const char *fmt,...) 
{           
    int i;
    char buf[256];
    va_list arg = (va_list)((char*)(&fmt)+ 4);
    i = vsprintf(buf, fmt, arg);
    Write(buf,itsStrlen(buf),ConsoleOutput);
    return i;
}
/*======================================================================*
                                 sprintf
{*======================================================================*/
void sprintf(char *buf,const char *fmt,...) 
{           
    va_list arg = (va_list)((char*)(&fmt)+ 4);
    vsprintf(buf, fmt, arg);
}

