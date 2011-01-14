# rawsrv/Makefile.am: part of meabench, an MEA recording and analysis tool
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


bin_PROGRAMS = rawsrv nssrv
noinst_HEADERS = Aux.H Defs.H RawSource.H RS_MCS.H RS_Sock.H UEIConfig.H rawsrv.C ChannelShift.H
#@MEAHARDWARE@srv nssrv
#EXTRA_PROGRAMS = ueisrv mcssrv 
#EXTRA_rawsrv_SOURCES = RS_MCS.C nop.C mcssrv.C
if UEIHARDWARE
rawsrv_SOURCES = nop.C
#rawsrv_SOURCES = RS_UEI.C ueisrv.C powerdaq32.c
endif
if MCSHARDWARE
rawsrv_SOURCES = RS_MCS.C mcssrv.C
endif
if NIHARDWARE
rawsrv_SOURCES = nop.C
endif
#ueisrv_SOURCES = ueisrv.C RS_UEI.C powerdaq32.c
nssrv_SOURCES = nssrv.C RS_Sock.C ChannelShift.C


include $(top_srcdir)/Makefile.base
