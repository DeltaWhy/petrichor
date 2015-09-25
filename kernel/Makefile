HOST?=i686-elf
TOOLSDIR?=../tools/cross/bin
CFLAGS?=-g
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

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Werror
CPPFLAGS:=$(CPPFLAGS) -D__is_petrichor_kernel -Iinclude -I. -isystem $(DESTDIR)$(INCLUDEDIR)
LDFLAGS:=$(LDFLAGS) --sysroot $(DESTDIR)
LIBS:=$(LIBS) -nostdlib -nostartfiles -nodefaultlibs -lk -lgcc

OBJS := $(patsubst %.S,%.o,$(shell find . -maxdepth 1 -name "*.S"))
OBJS += $(patsubst %.c,%.o,$(shell find . -maxdepth 1 -name "*.c"))
OBJS += $(patsubst %.c,%.o,$(shell find hw -name "*.c"))
OBJS += $(patsubst %.S,%.o,$(shell find hw -name "*.S"))
OBJS += $(patsubst %.c,%.o,$(shell find mem -name "*.c"))
OBJS += $(patsubst %.S,%.o,$(shell find mem -name "*.S"))
OBJS += $(patsubst %.c,%.o,$(shell find shell -name "*.c"))
OBJS += $(patsubst %.S,%.o,$(shell find shell -name "*.S"))
OBJS += $(patsubst %.c,%.o,$(shell find task -name "*.c"))
OBJS += $(patsubst %.S,%.o,$(shell find task -name "*.S"))

all: petrichor.elf

.PHONY: all clean install install-headers install-kernel

-include $(OBJS:.o=.d)

petrichor.elf: $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -MD -o $@ -std=c11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f petrichor.elf $(OBJS) $(OBJS:.o=.d) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -aTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: petrichor.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp petrichor.elf $(DESTDIR)$(BOOTDIR)
