#
#	Makefile for elksemu.
#

ifndef TOPDIR
$(error TOPDIR is not defined)
endif

# Default
ifeq ($(CFLAGS),)
CFLAGS=-O $(DEFS)
endif

# Pick between CPU emulation, with three possible options, one of these must be defined
# USE_VM86 - uses the kernel syscall vm86, note that this only works for x86-32 Linux systems
# USE_X86EMU - uses the libx86emu CPU emulation library
# USE_PTRACE - start a 16-bit protected mode child process and controls it via ptrace calls
USE_PTRACE=1

ifdef USE_VM86
CFLAGS+=-m32 -DUSE_VM86
else
ifdef USE_X86EMU
CFLAGS+=-DUSE_X86EMU=1 -lx86emu
else
ifdef USE_PTRACE
CFLAGS+=-DUSE_PTRACE=1
else
$(error One of USE_PTRACE, USE_X86EMU, USE_VM86 must be defined)
endif
endif
endif

# Turn on elkemu's strace like facility.
# DEFS=-DDEBUG

# For gcc making a.out with a basically ELF compiler
# CFLAGS=-O2 -fno-strength-reduce -b i486-linuxaout -N -s -static

OBJ=elks.o elks_sys.o elks_signal.o elks_pid.o minix.o
OBJ_STRACE=elks.o elks_sys_strace.o elks_signal.o elks_pid.o minix.o

all:	elksemu strace_elksemu

elksemu:	$(OBJ)
		$(CC) $(CFLAGS) -o $@ $^

strace_elksemu:	$(OBJ_STRACE)
		$(CC) $(CFLAGS) -o $@ $^

elks_sys.o:	call_tab.v defn_tab.v efile.h
$(OBJ):		elks.h

elks_sys_strace.o:	elks_sys.c elks.h call_tab.v defn_tab.v efile.h
		$(CC) $(CFLAGS) -DSTRACE=1 -c -o $@ $<

ifdef PREFIX
INSTALLED_CALL_TAB = $(PREFIX)/share/misc/elks/call_tab.v
INSTALLED_DEFN_TAB = $(PREFIX)/share/misc/elks/defn_tab.v
else
# Try to auto-detect the locations of the system call tables call_tab.v and
# defn_tab.v from elks-libc, if they are not already given by means of
# $(PREFIX).
#
# ia16-elf-gcc's target libraries are expected to go in PREFIX/ia16-elf/
# lib/, while the syscall tables should be in PREFIX/share/misc/elks/
# (PREFIX = ia16-elf-gcc's installation prefix).  So looking for "libraries"
# in ../../share/misc/elks/ relative to GCC's library directories should
# lead us on the right track.
INSTALLED_CALL_TAB:=$(shell \
		ia16-elf-gcc -print-file-name=../../share/misc/elks/call_tab.v)
INSTALLED_DEFN_TAB:=$(shell \
		ia16-elf-gcc -print-file-name=../../share/misc/elks/defn_tab.v)
ifeq "../../share/misc/elks/call_tab.v" "$(INSTALLED_CALL_TAB)"
INSTALLED_CALL_TAB:=
endif
ifeq "../../share/misc/elks/defn_tab.v" "$(INSTALLED_DEFN_TAB)"
INSTALLED_DEFN_TAB:=
endif
endif

# Copy call_tab.v and defn_tab.v from the elks-libc installation.  To do
# this, $(INSTALLED_CALL_TAB) and $(INSTALL_DEFN_TAB) must be defined.
#
# But do not throw an error if we do not actually need them --- for example,
# during a `make clean'.  See https://github.com/jbruchon/elks/issues/283 .
call_tab.v:	$(INSTALLED_CALL_TAB)
	$(if $(INSTALLED_CALL_TAB),, \
	    $(error cannot find call_tab.v from elks-libc installation))
	cp $< $@
defn_tab.v:	$(INSTALLED_DEFN_TAB)
	$(if $(INSTALLED_DEFN_TAB),, \
	    $(error cannot find defn_tab.v from elks-libc installation))
	cp $< $@

efile.h: $(TOPDIR)/elkscmd/rootfs_template/etc/perror
	sh mkefile $<

dummy:

# The kernel patch or module _requires_ this location but binfmt-misc is easy
# to redirect.
install: elksemu
	install -d $(DIST)/lib
	install -s -o root -g root -m 4555 elksemu $(DIST)/lib/elksemu

clean realclean:
	rm -f $(OBJ) $(OBJ_STRACE) binfmt_elks.o elksemu strace_elksemu call_tab.v defn_tab.v efile.h

module: binfmt_elks.o

# HOW to compile the module...
# BUT remember you don't need it for a recent 2.1.X; use binfmt_misc.

# This matches my compile (2.0.x); yours may be different.
MODCFLAGS=-D__KERNEL__ -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer \
          -fno-strength-reduce -pipe -m486 -DCPU=486 -DMODULE -DMODVERSIONS \
	  -include /usr/include/linux/modversions.h

binfmt_elks.o:	binfmt_elks.c
	gcc -c $(MODCFLAGS) binfmt_elks.c

# This is another option
#MODCFLAGS=-O -fomit-frame-pointer -DCPU=386 -D__KERNEL__ -DMODULE
# Or this ...
#MODCFLAGS=-O -fomit-frame-pointer -D__KERNEL__ -DMODULE -DCONFIG_MODVERSIONS

.PHONY:	all

