# rawsrv/nssrv.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = nssrv
include(../common/meabench.pri)

HEADERS = Aux.H ChannelShift.H Defs.H RawSource.H RS_Sock.H
SOURCES = ChannelShift.C nssrv.C RS_Sock.C


