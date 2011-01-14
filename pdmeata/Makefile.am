# pdmeata/Makefile.am: part of meabench, an MEA recording and analysis tool
# Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

EXTRA_PROGRAMS = pdmeata
EXTRA_pdmeata_SOURCES = pdmreset.c burntest.C testmmap.C testrun.C \
	testindma.C \
	testwait.C ueimap.txt pinmap.txt mapem.pl pdmeata.c pdmeata.h 
if UEIHARDWARE
data_DATA = pdmeata.o
noinst_PROGRAMS = burntest testmmap testrun testindma testwait
bin_PROGRAMS = pdmreset
pdmreset_SOURCES = pdmreset.c
burntest_SOURCES = burntest.C
testmmap_SOURCES = testmmap.C
testrun_SOURCES = testrun.C
testindma_SOURCES = testindma.C
testwait_SOURCES = testwait.C
BUILT_SOURCES = mapem.h
endif

mapem.h: ueimap.txt pinmap.txt mapem.pl
	mapem.pl

pdmeata.o: pdmeata.c pdmeata.h mapem.h
	gcc -c pdmeata.c -I@KERNEL_INC@ -I@POWERDAQ_INC@ -I@RTLINUX_INC@ -O -Wall -D__KERNEL__ -DMODULE