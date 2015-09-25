all: headers
	$(MAKE) -C libc install
	$(MAKE) -C kernel install
.PHONY: all image headers clean run debug
headers:
	mkdir -p sysroot
	$(MAKE) -C libc install-headers
	$(MAKE) -C kernel install-headers

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -rf sysroot
	rm -f petrichor-floppy.img

image: all petrichor-floppy.img

petrichor-floppy.img: sysroot/boot/petrichor.elf
	rm -f $@
	cp grub_dos.img $@
	mcopy -i $@ $^ ::boot

run: image
	tools/cross/bin/bochs -q; true

debug: image
	tools/cross/bin/bochs -q 'gdbstub: enabled=1'; true
