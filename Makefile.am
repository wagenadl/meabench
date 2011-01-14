# Makefile.am: part of meabench, an MEA recording and analysis tool
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

ACLOCAL_AMFLAGS=-I m4

if MCSHARDWARE
if COND_KERNEL_2_4
maybe_MCCARD_2_4 = MCCard-linux-2.4
endif
if COND_KERNEL_2_6
maybe_MCCARD_2_6 = MCCard-linux-2.6
endif
endif
if UEIHARDWARE
maybe_PDMEATA = pdmeata
endif

SUBDIRS=base common gui tools scripts \
	rawsrv spikesrv replay record \
	60hz artifilt extractwindow rms \
	scope raster flexraster spikesound \
	monitor \
	utils perl matlab doc \
	$(maybe_PDMEATA) \
	$(maybe_MCCARD_2_4) \
	$(maybe_MCCARD_2_6) \
	neurosock

noinst_HEADERS = Makefile.moc Makefile.base Makefile.gui Makefile.tools \
	announce.txt bootstrap

cleanup:; rm -f *~ */*~ */*/*~ *.bak */*.bak */*.moc.* */*.o */*D.? */*I.? doc/*.aux doc/*.log doc/user*.ps doc/user/*.pdf

targz: cleanup distclean
	( cd ..; tar cvzf meabench.tar.gz --exclude \*CVS\* meabench )

SALPAFILES=base/Timeref.H base/Error.H base/Sprintf.H base/dbx.H \
	base/minmax.H base/stringperror.H base/Sprintf.H \
	base/CyclBuf.H common/NoiseLevels.H base/Variance.H \
	base/dbx.C base/Sprintf.C base/stringperror.C \
	common/ChannelNrs.H common/Types.H common/Config.H \
	common/MedianVariance.H \
	artifilt/LocalFit.H \
	artifilt/LocalFit.C artifilt/posthocartifilt.C
solosalpa-build:;
	./solosalpa-grab ${SALPAFILES} 
	make -C solosalpa dist
