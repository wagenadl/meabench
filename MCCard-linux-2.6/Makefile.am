# MCCard/Makefile.am: part of meabench, an MEA recording and analysis tool
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
noinst_HEADERS = TODO AUTHOR README INSTALL ChangeLog \
	MCCard26.c MCCard26nodev.c MCCard.h Amcc.h Kbuild 45-MCCard.rules

noinst_DATA = MCCard.ko MCCardnodev.ko
noinst_PROGRAMS = hardrelease rawread forcefifo
hardrelease_SOURCES = hardrelease.C
rawread_SOURCES = rawread.c
forcefifo_SOURCES = forcefifo.C

MCCard.ko: modules MCCard26.c MCCard.h Amcc.h
MCCardnodev.ko: modules MCCard26nodev.c MCCard.h Amcc.h

modules::
	$(MAKE) -C @KERNEL_DIR@ M=`pwd` $@ modules

install-data-local: modules_install

install-data-hook:
	test -d /etc/udev/rules.d && { \
		$(INSTALL) -m 644 45-MCCard.rules /etc/udev/rules.d; \
		} || { \
		rm -f /dev/MCCard; \
		mknod /dev/MCCard c 250 0; \
		chmod a+rw /dev/MCCard; \
	}

modules_install::
	$(MAKE)  -C @KERNEL_DIR@ M=`pwd` $@ modules_install

clean::
	$(MAKE)  -C @KERNEL_DIR@ M=`pwd` $@ clean


