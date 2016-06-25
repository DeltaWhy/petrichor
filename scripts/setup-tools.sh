#!/bin/bash
set -euo pipefail
BINUTILS=binutils-2.25
GCC=gcc-5.2.0
NASM_VERSION=2.11.08
NASM=nasm-$NASM_VERSION
BOCHS_VERSION=2.6.8
BOCHS=bochs-$BOCHS_VERSION
ISL=isl-0.13
# dependencies for debian/ubuntu
if grep 'Debian' /etc/issue || grep 'Ubuntu' /etc/issue; then
    sudo apt-get install build-essential gcc texinfo libgmp-dev libmpfr-dev libmpc-dev
fi

# fetch sources
cd `dirname $0`
mkdir -p ../tools
cd ../tools
echo "Fetching sources..."
[ -f $BINUTILS.tar.bz2 ] || wget ftp://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.bz2
[ -f $GCC.tar.bz2 ] || wget ftp://ftp.gnu.org/gnu/gcc/$GCC/$GCC.tar.bz2
[ -f $NASM.tar.bz2 ] || wget http://www.nasm.us/pub/nasm/releasebuilds/$NASM_VERSION/$NASM.tar.bz2
[ -f $BOCHS.tar.gz ] || wget http://sourceforge.net/projects/bochs/files/bochs/$BOCHS_VERSION/$BOCHS.tar.gz
[ -f $ISL.tar.gz ] || wget http://isl.gforge.inria.fr/isl-0.13.tar.gz
#[ -d $BINUTILS ] || (tar xf $BINUTILS.tar.bz2 && sed -i -e 's/doc@cygnus.com/doc@@cygnus.com/' $BINUTILS/bfd/doc/bfd.texinfo)
echo "Extracting sources..."
[ -d $BINUTILS ] || tar xf $BINUTILS.tar.bz2
[ -d $GCC ] || tar xf $GCC.tar.bz2
[ -d $NASM ] || tar xf $NASM.tar.bz2
[ -d $BOCHS ] || tar xf $BOCHS.tar.gz
[ -d $ISL ] || tar xf $ISL.tar.gz

export PREFIX=$PWD/cross
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"


if [ ! -d build-isl ]; then
    echo -e "\n#############"
    echo "Building isl"
    echo -e "##############\n"
    mkdir -p build-isl
    cd build-isl
    ../$ISL/configure --target=$TARGET --prefix="$PREFIX"
    make
    echo -e "\n#############"
    echo "Installing isl"
    echo -e "##############\n"
    make install
    cd ..
fi

if [ ! -d build-binutils ]; then
    echo -e "\n#################"
    echo "Building binutils"
    echo -e "#################\n"
    mkdir -p build-binutils
    cd build-binutils
    ../$BINUTILS/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    echo -e "\n###################"
    echo "Installing binutils"
    echo -e "###################\n"
    make install
    cd ..
fi

if [ ! -d build-gcc ]; then
    which -- $TARGET-as > /dev/null || (echo $TARGET-as is not in the PATH && false)
    echo -e "\n############"
    echo "Building gcc"
    echo -e "############\n"
    mkdir -p build-gcc
    cd build-gcc
    ../$GCC/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers --with-isl=$PREFIX
    make all-gcc
    make all-target-libgcc
    echo -e "\n##############"
    echo "Installing gcc"
    echo -e "##############\n"
    make install-gcc
    make install-target-libgcc
    cd ..
fi

if [ ! -f $NASM/nasm ]; then
    echo -e "\n#############"
    echo "Building nasm"
    echo -e "#############\n"
    cd $NASM
    ./configure --target=$TARGET --prefix="$PREFIX"
    make
    echo -e "\n###############"
    echo "Installing nasm"
    echo -e "###############\n"
    make install
    cd ..
fi

if [ ! -d build-bochs ]; then
    echo -e "\n##############"
    echo "Building bochs"
    echo -e "##############\n"
    mkdir -p build-bochs
    cd build-bochs
    ../$BOCHS/configure --prefix="$PREFIX" --enable-sb16 --enable-x86-debugger --enable-gdb-stub
    make
    echo -e "\n################"
    echo "Installing bochs"
    echo -e "################\n"
    make install
    cd ..
fi

echo -e "\n##################################"
echo "Toolchain setup complete."
