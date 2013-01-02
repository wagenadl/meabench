# base/base.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = lib
TARGET = meabase
CONFIG += dll
include(../common/meabench.pri)
LIBS -= -lmeabase
DESTDIR = $$DESTBASE/lib

HEADERS = \
	Cmdr.H \
	CorrelData.H \
	CyclBuf.H \
	dbx.H \
	Description.H \
	doubletime.H \
	Error.H \
	Foreach.H \
	InputMUX.H \
	Linebuf.H \
	minmax.H \
	SFACli.H \
	SFCli.H \
	SFCVoid.H \
	SFFreeze.H \
	SFHeader.H \
	SFSrv.H \
	SFSVoid.H \
	ShmCli.H \
	ShmSrv.H \
	Sigint.H \
	Sockclient.H \
	Sockserver.H \
	Sprintf.H \
	Stdin2Sock.H \
	stringperror.H \
	Timeref.H \
	Variance.H \
	WakeupCli.H \
	WakeupCodes.H \
	WakeupSrv.H

SOURCES = \
	Cmdr.C \
	dbx.C \
	Description.C \
	Linebuf.C \
	ShmCli.C \
	ShmSrv.C \
	Sigint.C \
	Sockclient.C \
	Sockserver.C \
	Sprintf.C \
	stringperror.C \
	WakeupCli.C \
	WakeupSrv.C
	
