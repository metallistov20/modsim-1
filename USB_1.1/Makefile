#
# (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com
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
#

# Prefix to UCLIBS crosscompiler, may be commented-out in which case it won't be cross- rather a _reguolar compiation
PREFIX=m68k-pic-coff-

# (cross-)compiler
CC=$(PREFIX)gcc

# Headers. Can be omitted as long as we use toolchaing installed into </opt> we don't pass <-L> and <-rpath-link>
CFLAGS=-O3 -DUCSIMM -DQUASIFLOAT -I/opt/uClinux/m68k-pic-coff/include

# Excessive debug info not needed when program is ready. Spoils 'realtime' operating mode. Keep commented-out.
# CFLAGS+=-DDEBUG_DATA

# USB 2.0 signal levels for parsing CVS file data. Comment-out to parse CSV file data according to USB 1.0 signal levels.
CFLAGS+= -DUSB20

# Checking Data-IN (backward data stream on D-); expected to be same as X(CH2) in raw data (CSV file) 
# CFLAGS+= -DDIN_FEEDBACK

# As long as we use toolchaing installed into </opt> we don't pass <-L> and <-rpath-link>
LDFLAGS=

.o: .c
	$(CC) $(CFLAGS)  -o $@ -c $< $(CFLAGS) $(LDFLAGS)

all:	m

m: modsim.o dstruct.o dport.o
	$(CC) $(CFLAGS)  -o m modsim.o dstruct.o dport.o

clean:
	rm *.o *~ modsim *.coff m
