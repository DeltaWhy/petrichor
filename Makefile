all: headers
	$(MAKE) -C libc install
	$(MAKE) -C kernel install
.PHONY: all install headers clean
headers:
	mkdir -p sysroot
	$(MAKE) -C libc install-headers
	$(MAKE) -C kernel install-headers

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -rf sysroot
