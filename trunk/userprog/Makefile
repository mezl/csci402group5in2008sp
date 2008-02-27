# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the multiprogramming assignment
#    Defines set up assuming multiprogramming is done before the file system.
#    If not, use the "filesys first"  defines below. 
#
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

DEFINES = -DUSER_PROGRAM -DFILESYS_NEEDED -DFILESYS_STUB
INCPATH = -I../bin -I../filesys -I../userprog -I../threads -I../machine
HFILES = $(THREAD_H) $(USERPROG_H)
CFILES = $(THREAD_C) $(USERPROG_C)
C_OFILES = $(THREAD_O) $(USERPROG_O)

# if file sys done first!
# DEFINES = -DUSER_PROGRAM -DFILESYS_NEEDED -DFILESYS
# INCPATH = -I../bin -I../filesys -I../userprog -I../threads -I../machine
# HFILES = $(THREAD_H) $(USERPROG_H) $(FILESYS_H)
# CFILES = $(THREAD_C) $(USERPROG_C) $(FILESYS_C)
# C_OFILES = $(THREAD_O) $(USERPROG_O) $(FILESYS_O)

include ../Makefile.common
include ../Makefile.dep
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
main.o: ../threads/main.cc ../threads/copyright.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/system.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/scheduler.h ../threads/list.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h
list.o: ../threads/list.cc ../threads/copyright.h ../threads/list.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h
scheduler.o: ../threads/scheduler.cc ../threads/copyright.h \
  ../threads/scheduler.h ../threads/list.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/system.h ../machine/interrupt.h ../machine/stats.h \
  ../machine/timer.h
synch.o: ../threads/synch.cc ../threads/copyright.h ../threads/synch.h \
  ../threads/thread.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/machine.h ../machine/translate.h ../machine/disk.h \
  ../userprog/addrspace.h ../filesys/filesys.h ../filesys/openfile.h \
  ../userprog/table.h ../userprog/bitmap.h ../threads/list.h \
  ../threads/system.h ../threads/scheduler.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h
synchlist.o: ../threads/synchlist.cc ../threads/copyright.h \
  ../threads/synchlist.h ../threads/list.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h
system.o: ../threads/system.cc ../threads/copyright.h ../threads/system.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h
thread.o: ../threads/thread.cc ../threads/copyright.h ../threads/thread.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/machine.h ../machine/translate.h ../machine/disk.h \
  ../userprog/addrspace.h ../filesys/filesys.h ../filesys/openfile.h \
  ../userprog/table.h ../userprog/bitmap.h ../threads/switch.h \
  ../threads/synch.h ../threads/list.h ../threads/system.h \
  ../threads/scheduler.h ../machine/interrupt.h ../machine/stats.h \
  ../machine/timer.h
utility.o: ../threads/utility.cc ../threads/copyright.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h
threadtest.o: ../threads/threadtest.cc ../threads/copyright.h \
  ../threads/system.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h
test_code.o: ../threads/test_code.cc ../threads/copyright.h \
  ../threads/system.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../threads/synch.h \
  ../threads/manager.cc ../threads/customer.h ../threads/cline.h \
  ../threads/clerk.h ../threads/ctable.h ../threads/appclerk.cc \
  ../threads/picclerk.cc ../threads/passclerk.cc ../threads/cashclerk.cc
cline.o: ../threads/cline.cc ../threads/cline.h ../threads/list.h \
  ../threads/copyright.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h
customer.o: ../threads/customer.cc ../threads/customer.h \
  ../threads/bool.h ../threads/copyright.h ../threads/system.h \
  ../threads/utility.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../threads/synch.h \
  ../threads/cline.h
ctable.o: ../threads/ctable.cc ../threads/ctable.h ../threads/list.h \
  ../threads/copyright.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h
clerk.o: ../threads/clerk.cc ../threads/clerk.h ../threads/cline.h \
  ../threads/list.h ../threads/copyright.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
  ../threads/ctable.h ../threads/customer.h
appclerk.o: ../threads/appclerk.cc ../threads/clerk.h ../threads/cline.h \
  ../threads/list.h ../threads/copyright.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
  ../threads/ctable.h ../threads/customer.h
picclerk.o: ../threads/picclerk.cc ../threads/clerk.h ../threads/cline.h \
  ../threads/list.h ../threads/copyright.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
  ../threads/ctable.h ../threads/customer.h
passclerk.o: ../threads/passclerk.cc ../threads/clerk.h \
  ../threads/cline.h ../threads/list.h ../threads/copyright.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
  ../threads/ctable.h ../threads/customer.h
cashclerk.o: ../threads/cashclerk.cc ../threads/clerk.h \
  ../threads/cline.h ../threads/list.h ../threads/copyright.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/synch.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/scheduler.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
  ../threads/ctable.h ../threads/customer.h
manager.o: ../threads/manager.cc
office.o: ../threads/office.cc ../threads/office.h ../threads/manager.cc \
  ../threads/customer.h ../threads/bool.h ../threads/copyright.h \
  ../threads/system.h ../threads/utility.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../threads/synch.h \
  ../threads/cline.h ../threads/clerk.h ../threads/ctable.h
interrupt.o: ../machine/interrupt.cc ../threads/copyright.h \
  ../machine/interrupt.h ../threads/list.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/system.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/scheduler.h ../machine/stats.h \
  ../machine/timer.h
sysdep.o: ../machine/sysdep.cc ../threads/copyright.h \
  /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/feature_tests.h /usr/include/sys/isa_defs.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  /usr/include/unistd.h /usr/include/sys/unistd.h /usr/include/fcntl.h \
  /usr/include/sys/stat.h /usr/include/sys/stat_impl.h \
  /usr/include/sys/fcntl.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/signal.h \
  /usr/include/iso/signal_iso.h /usr/include/sys/iso/signal_iso.h \
  /usr/include/sys/signal.h /usr/include/sys/ucontext.h \
  /usr/include/sys/regset.h /usr/include/v7/sys/privregs.h \
  /usr/include/v7/sys/psr.h /usr/include/sys/fsr.h /usr/include/errno.h \
  /usr/include/sys/errno.h /usr/include/sys/un.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/sys/netconfig.h /usr/include/netinet/in.h \
  /usr/include/sys/stream.h /usr/include/sys/vnode.h \
  /usr/include/sys/t_lock.h /usr/include/sys/machlock.h \
  /usr/include/sys/param.h /usr/include/sys/mutex.h \
  /usr/include/sys/rwlock.h /usr/include/sys/semaphore.h \
  /usr/include/sys/condvar.h /usr/include/sys/rwstlock.h \
  /usr/include/sys/ksynch.h /usr/include/sys/cred.h \
  /usr/include/vm/seg_enum.h /usr/include/sys/poll.h \
  /usr/include/sys/strmdep.h /usr/include/sys/model.h \
  /usr/include/sys/strft.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/byteorder.h \
  /usr/include/net/if_dl.h /usr/include/sys/mman.h ../machine/interrupt.h \
  ../threads/list.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h ../threads/system.h ../threads/thread.h \
  ../machine/machine.h ../machine/translate.h ../machine/disk.h \
  ../userprog/addrspace.h ../filesys/filesys.h ../filesys/openfile.h \
  ../userprog/table.h ../userprog/bitmap.h ../threads/scheduler.h \
  ../machine/stats.h ../machine/timer.h
stats.o: ../machine/stats.cc ../threads/copyright.h ../threads/utility.h \
  ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/stats.h
timer.o: ../machine/timer.cc ../threads/copyright.h ../machine/timer.h \
  ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/system.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/scheduler.h ../threads/list.h \
  ../machine/interrupt.h ../machine/stats.h
addrspace.o: ../userprog/addrspace.cc ../threads/system.h \
  ../threads/copyright.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../bin/noff.h ../threads/synch.h
bitmap.o: ../userprog/bitmap.cc ../threads/copyright.h \
  ../userprog/bitmap.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../filesys/openfile.h
exception.o: ../userprog/exception.cc ../threads/copyright.h \
  ../threads/system.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../userprog/syscall.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/iostream \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/c++config.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/os_defines.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/ostream \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/ios \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/iosfwd \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/c++locale.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/clocale /usr/include/locale.h \
  /usr/include/iso/locale_iso.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/libintl.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cctype /usr/include/ctype.h \
  /usr/include/iso/ctype_iso.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stringfwd.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/fpos.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/c++io.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cstdio \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cstddef \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stddef.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/gthr.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/gthr-default.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/pthread.h \
  /usr/include/sched.h /usr/include/unistd.h /usr/include/sys/unistd.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cwchar \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/ctime /usr/include/wchar.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/wchar_iso.h \
  /usr/include/wchar_impl.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/functexcept.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/exception_defines.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/exception \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/char_traits.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cstring \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/localefwd.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/ios_base.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/atomicity.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/locale_classes.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/string \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/memory \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_algobase.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/climits \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/limits.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/syslimits.h \
  /usr/include/iso/limits_iso.h /usr/include/sys/int_limits.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cstdlib \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/new \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_pair.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/type_traits.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_iterator_base_types.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_iterator_base_funcs.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/concept_check.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_iterator.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_alloc.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_threads.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_construct.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_uninitialized.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_raw_storage_iter.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_function.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/basic_string.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/algorithm \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_algo.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_heap.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/stl_tempbuf.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/basic_string.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/streambuf \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/streambuf.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/basic_ios.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/streambuf_iterator.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/locale_facets.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cwctype /usr/include/wctype.h \
  /usr/include/iso/wctype_iso.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/ctype_base.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/ctype_inline.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/codecvt.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/time_members.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/sparc-sun-solaris2.8/bits/messages_members.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/basic_ios.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/ostream.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/locale \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/locale_facets.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cerrno /usr/include/errno.h \
  /usr/include/sys/errno.h /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/cmath \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/math.h \
  /usr/include/iso/math_iso.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/floatingpoint.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/ieeefp.h \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/cmath.tcc \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/limits \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/typeinfo \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/istream \
  /usr/usc/gnu/gcc/3.3.2/include/c++/3.3.2/bits/istream.tcc \
  ../threads/synch.h
progtest.o: ../userprog/progtest.cc ../threads/copyright.h \
  ../threads/system.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/thread.h ../machine/machine.h ../machine/translate.h \
  ../machine/disk.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h ../machine/console.h \
  ../threads/synch.h
console.o: ../machine/console.cc ../threads/copyright.h \
  ../machine/console.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../threads/system.h ../threads/thread.h ../machine/machine.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/scheduler.h ../threads/list.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h
machine.o: ../machine/machine.cc ../threads/copyright.h \
  ../machine/machine.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/translate.h ../machine/disk.h ../threads/system.h \
  ../threads/thread.h ../userprog/addrspace.h ../filesys/filesys.h \
  ../filesys/openfile.h ../userprog/table.h ../userprog/bitmap.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h
mipssim.o: ../machine/mipssim.cc ../threads/copyright.h \
  ../machine/machine.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/translate.h ../machine/disk.h ../machine/mipssim.h \
  ../threads/system.h ../threads/thread.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/scheduler.h ../threads/list.h \
  ../machine/interrupt.h ../machine/stats.h ../machine/timer.h
translate.o: ../machine/translate.cc ../threads/copyright.h \
  ../machine/machine.h ../threads/utility.h ../threads/bool.h \
  ../machine/sysdep.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdarg.h \
  /usr/include/iso/stdio_iso.h /usr/include/sys/feature_tests.h \
  /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/stdio_tag.h \
  /usr/include/stdio_impl.h /usr/include/string.h \
  /usr/include/iso/string_iso.h /usr/include/stdlib.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/iso/stdlib_iso.h \
  /usr/include/sys/wait.h \
  /usr/usc/gnu/gcc/3.3.2/lib/gcc-lib/sparc-sun-solaris2.8/3.3.2/include/sys/types.h \
  /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
  /usr/include/sys/select.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/iso/time_iso.h /usr/include/sys/time_impl.h \
  /usr/include/sys/resource.h /usr/include/sys/siginfo.h \
  /usr/include/sys/machsig.h /usr/include/sys/procset.h \
  ../machine/translate.h ../machine/disk.h ../userprog/addrspace.h \
  ../filesys/filesys.h ../filesys/openfile.h ../userprog/table.h \
  ../userprog/bitmap.h ../threads/system.h ../threads/thread.h \
  ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
  ../machine/stats.h ../machine/timer.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
