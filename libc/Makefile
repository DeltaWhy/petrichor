HOST?=i686-elf
TOOLSDIR?=../tools/cross/bin
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

AR=$(TOOLSDIR)/$(HOST)-ar
AS=$(TOOLSDIR)/$(HOST)-as
CC=$(TOOLSDIR)/$(HOST)-gcc

DESTDIR?=../sysroot
PREFIX?=/usr
EXEC_PREFIX?=
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_petrichor_libc -Iinclude -isystem $(DESTDIR)$(INCLUDEDIR)
LIBK_CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_petrichor_kernel

FREEOBJS:=\
string.o

HOSTEDOBJS:=

OBJS:=$(FREEOBJS) $(HOSTEDOBJS)
LIBK_OBJS:=$(FREEOBJS:.o=.libk.o)

BINARIES=libc.a libk.a

all: $(BINARIES)

.PHONY: all clean install install-headers install-libs

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.libk.o: %.S
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
