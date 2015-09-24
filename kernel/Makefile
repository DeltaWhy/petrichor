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
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_petrichor_kernel -Iinclude -isystem $(DESTDIR)$(INCLUDEDIR)
LDFLAGS:=$(LDFLAGS) --sysroot $(DESTDIR)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

OBJS:=\
kernel.o \
loader.o \

all: petrichor.elf

.PHONY: all clean install install-headers install-kernel

petrichor.elf: $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f petrichor.elf $(OBJS) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: petrichor.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp petrichor.elf $(DESTDIR)$(BOOTDIR)
