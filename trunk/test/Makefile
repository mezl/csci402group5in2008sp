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

all: office test halt matmult sort testfiles test_fork lockcond lockcondbreak combinetest

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.s > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

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
lockcondbreak.o: lockcondbreak.c
	$(CC2C) lockcondbreak.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o lockcondbreak.o
	rm -f tmp.c
lockcondbreak: lockcondbreak.o start.o
	$(LD) $(LDFLAGS) start.o lockcondbreak.o -o lockcondbreak.coff
	../bin/coff2noff lockcondbreak.coff lockcondbreak
office.o: office.c
	$(CC2C) office.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o office.o
	rm -f tmp.c
office: office.o start.o
	$(LD) $(LDFLAGS) start.o office.o -o office.coff
	../bin/coff2noff office.coff office

test.o: test.c
	$(CC2C) test.c > tmp.c
	$(CC) $(CFLAGS) -c tmp.c -o test.o
	rm -f tmp.c
test: test.o start.o
	$(LD) $(LDFLAGS) start.o test.o -o test.coff
	../bin/coff2noff test.coff test
   
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
combinetest.o: combinetest.c
	$(CC) $(CFLAGS) -c combinetest.c
combinetest: combinetest.o start.o
	$(LD) $(LDFLAGS) start.o combinetest.o -o combinetest.coff
	../bin/coff2noff combinetest.coff combinetest

clean:
	rm -f *.o *.coff
