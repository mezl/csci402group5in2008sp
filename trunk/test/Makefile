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

all: multi_office project2_final office halt matmult sort testfiles test_exec test_fork lockcond test_exit test_matmult simplelock testCond1 testCond2  

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
clean:
	rm -f test_exit test_matmult 
	rm -f *.o *.coff
	rm -f multi_office project2_final office halt matmult sort testfiles test_exec test_fork lockcond simplelock testCond1 testCond2
