# tools/tools.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = lib
TARGET = meatools
CONFIG += dll
include(../common/meabench.pri)
LIBS -= -lmeatools
DESTDIR = $$DESTBASE/lib

HEADERS = \
    Argh.H       Cumulate.H  MultiTimeQueue.H  SpikeQueue.H    SXCLog.H \
    BareSpike.H  ETA.H       SortingQueue.H    SpikeXCorrel.H
 
SOURCES = Argh.C  ETA.C  SpikeXCorrel.C  SXCLog.C
