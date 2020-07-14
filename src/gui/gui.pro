# gui/gui.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = lib
TARGET = meagui
CONFIG += dll
include(../common/meabench.pri)
CONFIG += qt
QT += widgets
DESTDIR = $$DESTBASE/lib

HEADERS = HexMEA.H  JNMEA.H \
    QGraph.H QMultiGraph.H

SOURCES =  QGraph.C QMultiGraph.C
