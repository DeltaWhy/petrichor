Petrichor OS
============
An operating system for x86. This is an educational project, use at your own risk.

## Toolchain setup
This project has only been tested on Arch Linux using GCC 5.2.0. You will probably need a GCC at least that recent to build the cross-compiler. You also need libgmp, libmpfr, libmpc, and their development versions.

Once you have all the dependencies, run `scripts/setup-tools.sh` and it should build a cross-compiler for you and install it into `tools/cross`. This script builds **binutils**, **gcc** (C frontend only), **nasm**, and **bochs**.

## Project structure
The kernel proper is found in `kernel`. The C library (the part we've implemented at least) is in `libc`. Two files are built from this: `libc.a` which contains everything and is suitable for linking user-space programs against, and `libk.a` which contains only freestanding functions (i.e. those that don't depend on syscalls) and is linked into the kernel proper.

## Coding style
Run `scripts/install-hooks.sh` to set up a pre-commit hook that will do some sanity checks.

Set your editor to indent C and assembly code with four spaces, no hard tabs. Avoid unbracketed if statements and loops, unless the whole thing fits on one line. For everything else, install astyle from your package manager and run `scripts/indent.sh` on the file in question or `scripts/indent-all.sh` to do the whole repo at once.
