# base/Makefile.am: part of meabench, an MEA recording and analysis tool
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

lib_LTLIBRARIES = libmea.la
libmea_la_SOURCES = ShmCli.C ShmSrv.C stringperror.C Sprintf.C dbx.C \
	Sigint.C Cmdr.C WakeupCli.C WakeupSrv.C Sockserver.C Sockclient.C \
	Linebuf.C Description.C # QuickRec.C
noinst_HEADERS = Cmdr.H CorrelData.H CyclBuf.H dbx.H Description.H doubletime.H Error.H Foreach.H InputMUX.H Linebuf.H minmax.H SFACli.H SFCli.H SFCVoid.H SFFreeze.H SFHeader.H SFSrv.H SFSVoid.H ShmCli.H ShmSrv.H Sigint.H Sockclient.H Sockserver.H Sprintf.H Stdin2Sock.H stringperror.H Timeref.H Variance.H WakeupCli.H WakeupCodes.H WakeupSrv.H
libmea_la_LDFLAGS = -release @VERSION@
INCLUDES=-I..
