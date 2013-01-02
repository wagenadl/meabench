# common/meabench.pri - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

CONFIG += debug
INCLUDEPATH += ..
MOC_DIR = .moc
OBJECTS_DIR = .obj
CONFIG -= qt
DESTBASE = ../meabench
DESTDIR = $$DESTBASE/bin
LIBS += -L$$DESTBASE/lib
LIBS += -lmeabase
LIBS += -lpthread
