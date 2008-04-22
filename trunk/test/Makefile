# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# If you are running on SunOS 5.x machines, uncomment the next line
GCCDIR = /auto/home-scf-22/csci402/testgnu/decstation-ultrix/bin/
LDFLAGS = -T newscript -N
#LDFLAGS = -N
ASFLAGS = -mips1
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)


CC = $(GCCDIR)gcc
AS = $(GCCDIR)as
LD = $(GCCDIR)ld
CPP = $(GCCDIR)cpp
GPP = g++
CC2C = ./cpp2c.pl
INCDIR =-I../userprog -I../threads  
CFLAGS = -G 0 -c $(INCDIR)
CCFLAGS =  -G 0 -c $(INCDIR)

all: multi_office project2_final office halt matmult sort testfiles test_exec test_fork lockcond \
test_exit test_matmult simplelock testCond1 testCond2 test_clerk test_customer \
multi_clerk multi_customer pic_clerk pass_clerk cash_clerk app_clerk

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.s > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

multi_office.o: multi_office.c
	$(CC2C) multi_office.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o multi_office.o
	rm -f tmp.c
multi_office: multi_office.o start.o
	$(LD) $(LDFLAGS) start.o multi_office.o -o multi_office.coff
	../bin/coff2noff multi_office.coff multi_office
project2_final.o: project2_final.c
	$(CC2C) project2_final.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o project2_final.o
	rm -f tmp.c
project2_final: project2_final.o start.o
	$(LD) $(LDFLAGS) start.o project2_final.o -o project2_final.coff
	../bin/coff2noff project2_final.coff project2_final
test_exec.o: test_exec.c
	$(CC2C) test_exec.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_exec.o
	rm -f tmp.c
test_exec: test_exec.o start.o
	$(LD) $(LDFLAGS) start.o test_exec.o -o test_exec.coff
	../bin/coff2noff test_exec.coff test_exec
test_fork.o: test_fork.c
	$(CC2C) test_fork.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_fork.o
	rm -f tmp.c
test_fork: test_fork.o start.o
	$(LD) $(LDFLAGS) start.o test_fork.o -o test_fork.coff
	../bin/coff2noff test_fork.coff test_fork
lockcond.o: lockcond.c
	$(CC2C) lockcond.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o lockcond.o
	rm -f tmp.c
lockcond: lockcond.o start.o
	$(LD) $(LDFLAGS) start.o lockcond.o -o lockcond.coff
	../bin/coff2noff lockcond.coff lockcond
simplelock.o: simplelock.c
	$(CC2C) simplelock.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o simplelock.o
	rm -f tmp.c
simplelock: simplelock.o start.o
	$(LD) $(LDFLAGS) start.o simplelock.o -o simplelock.coff
	../bin/coff2noff simplelock.coff simplelock
testCond1.o: testCond1.c
	$(CC2C) testCond1.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o testCond1.o
	rm -f tmp.c
testCond1: testCond1.o start.o
	$(LD) $(LDFLAGS) start.o testCond1.o -o testCond1.coff
	../bin/coff2noff testCond1.coff testCond1
testCond2.o: testCond2.c
	$(CC2C) testCond2.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o testCond2.o
	rm -f tmp.c
testCond2: testCond2.o start.o
	$(LD) $(LDFLAGS) start.o testCond2.o -o testCond2.coff
	../bin/coff2noff testCond2.coff testCond2
office.o: office.c
	$(CC2C) office.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o office.o
	rm -f tmp.c
office: office.o start.o
	$(LD) $(LDFLAGS) start.o office.o -o office.coff
	../bin/coff2noff office.coff office
   
halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

testfiles.o: testfiles.c
	$(CC) $(CFLAGS) -c testfiles.c
testfiles: testfiles.o start.o
	$(LD) $(LDFLAGS) start.o testfiles.o -o testfiles.coff
	../bin/coff2noff testfiles.coff testfiles
test_exit.o: test_exit.c
	$(CC2C) test_exit.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_exit.o
	rm -f tmp.c
test_exit: test_exit.o start.o
	$(LD) $(LDFLAGS) start.o test_exit.o -o test_exit.coff
	../bin/coff2noff test_exit.coff test_exit

test_matmult.o: test_matmult.c
	$(CC2C) test_matmult.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_matmult.o
	rm -f tmp.c
test_matmult: test_matmult.o start.o
	$(LD) $(LDFLAGS) start.o test_matmult.o -o test_matmult.coff
	../bin/coff2noff test_matmult.coff test_matmult
test_clerk.o: test_clerk.c
	$(CC2C) test_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_clerk.o
	rm -f tmp.c
test_clerk: test_clerk.o start.o
	$(LD) $(LDFLAGS) start.o test_clerk.o -o test_clerk.coff
	../bin/coff2noff test_clerk.coff test_clerk
test_customer.o: test_customer.c
	$(CC2C) test_customer.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test_customer.o
	rm -f tmp.c
test_customer: test_customer.o start.o
	$(LD) $(LDFLAGS) start.o test_customer.o -o test_customer.coff
	../bin/coff2noff test_customer.coff test_customer
multi_customer.o: multi_customer.c
	$(CC2C) multi_customer.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o multi_customer.o
	rm -f tmp.c
multi_customer: multi_customer.o start.o
	$(LD) $(LDFLAGS) start.o multi_customer.o -o multi_customer.coff
	../bin/coff2noff multi_customer.coff multi_customer
multi_clerk.o: multi_clerk.c
	$(CC2C) multi_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o multi_clerk.o
	rm -f tmp.c
multi_clerk: multi_clerk.o start.o
	$(LD) $(LDFLAGS) start.o multi_clerk.o -o multi_clerk.coff
	../bin/coff2noff multi_clerk.coff multi_clerk
app_clerk.o: app_clerk.c
	$(CC2C) app_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o app_clerk.o
	rm -f tmp.c
app_clerk: app_clerk.o start.o
	$(LD) $(LDFLAGS) start.o app_clerk.o -o app_clerk.coff
	../bin/coff2noff app_clerk.coff app_clerk
pic_clerk.o: pic_clerk.c
	$(CC2C) pic_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o pic_clerk.o
	rm -f tmp.c
pic_clerk: pic_clerk.o start.o
	$(LD) $(LDFLAGS) start.o pic_clerk.o -o pic_clerk.coff
	../bin/coff2noff pic_clerk.coff pic_clerk
pass_clerk.o: pass_clerk.c
	$(CC2C) pass_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o pass_clerk.o
	rm -f tmp.c
pass_clerk: pass_clerk.o start.o
	$(LD) $(LDFLAGS) start.o pass_clerk.o -o pass_clerk.coff
	../bin/coff2noff pass_clerk.coff pass_clerk
cash_clerk.o: cash_clerk.c
	$(CC2C) cash_clerk.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o cash_clerk.o
	rm -f tmp.c
cash_clerk: cash_clerk.o start.o
	$(LD) $(LDFLAGS) start.o cash_clerk.o -o cash_clerk.coff
	../bin/coff2noff cash_clerk.coff cash_clerk
clean:
	rm -f multi_clerk multi_customer
	rm -f test_clerk test_customer
	rm -f app_clerk pic_clerk pass_clerk cash_clerk
	rm -f test_exit test_matmult 
	rm -f *.o *.coff
	rm -f multi_office project2_final office halt matmult sort testfiles test_exec test_fork lockcond simplelock testCond1 testCond2
