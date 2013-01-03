# neurosock/neurosock.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = neurosock
include(../common/meabench.pri)

HEADERS = Defs.H   FrameBuffer.H  Mutex.H  \
    Error.H  MedianFlt.H    Neurosock.H  srvMeata.H
SOURCES = FrameBuffer.C  Neurosock.C  neurosockmain.C

mcshardware {
    HEADERS += srvMCMeata.H
    SOURCES += srvMCMeata.C
}

ueihardware {
    HEADERS += srvPdMeata.H
    SOURCES += srvPdMeata.C
}