# 
# (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA






export TARGET="arm-tplink-eabi"
export BHTYPE="x86_64-pc-linux-gnu"
export PREFIX="/opt/xGCC-"$TARGET"-baremetal"



###########################################################################################
# 1. Download binutils-2.25.tar.gz, gcc-4.9.3.tar.gz, newlib-2.2.0-1.tar.gz fro internet

#	wget http://ftp.yz.yamagata-u.ac.jp/pub/GNU/binutils/binutils-2.26.tar.gz
#	wget http://mirrors.dotsrc.org/gnu/gcc/gcc-4.9.3/gcc-4.9.3.tar.gz
#	wget ftp://sourceware.org/pub/newlib/newlib-2.2.0-1.tar.gz

###########################################################################################
# 2. Unpack binutils-2.25.tar.gz, gcc-4.9.3.tar.gz, newlib-2.2.0-1.tar.gz fro internet

	tar xzvf binutils-2.26.tar.gz
	tar xzvf gcc-4.9.3.tar.gz
	tar xzvf newlib-2.2.0-1.tar.gz

###########################################################################################

# 3. Doing BINUTILS

	export BINUTILS=binutils-2.26

	mkdir build-$BINUTILS

	cd build-$BINUTILS

		../binutils-2.26/configure --build=$BHTYOE --target=$TARGET  --prefix=$PREFIX

		make all

		make install

	cd ..

###########################################################################################

# 3. Doing first GCC

	export GCC=gcc-4.9.3

	export PATH=$PATH:$PREFIX/bin

	mkdir build-$GCC

	cd ./build-$GCC

		../$GCC/configure --target=$TARGET  --prefix=$PREFIX  --without-headers --with-newlib  --with-gnu-as  --with-gnu-ld

		make all-gcc

		make install-gcc

	cd ..

###########################################################################################

# 3. Doing baremetal libraries NEWLIB with GCC generated at stage 2.

	export NEWLIB=newlib-2.2.0-1

	mkdir build-$NEWLIB

	cd build-$NEWLIB

		../$NEWLIB/configure --target=$TARGET  --prefix=$PREFIX

		make all

		make install

	cd ..

###########################################################################################

# 4. Doing baremetal GCC - compiling it against baremetal libraries NEWLIB generated at stage 3.

	rm -rf $GCC

	tar xzvf $GCC.tar.gz

	rm -rf build-$GCC

	mkdir build-$GCC

	cd build-$GCC

		../$GCC/configure --target=$TARGET  --prefix=$PREFIX  --with-newlib  --with-gnu-as  --with-gnu-ld  --disable-shared --disable-libssp --enable-languages=c,c++

		make all -j 3

		make install

	cd ..

exit

