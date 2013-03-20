# common/meabench.pri - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

include("../../config.pri")

CONFIG += debug
INCLUDEPATH += ..
INCLUDEPATH += ../driver
DEPENDPATH += ../common
DEPENDPATH += ../base
DEPENDPATH += common
DEPENDPATH += base
DEPENDPATH += ../driver
DEPENDPATH += driver
MOC_DIR = .moc
OBJECTS_DIR = .obj
CONFIG -= qt
DESTBASE = ../../build
DESTDIR = $$DESTBASE/bin
LIBS += -L$$DESTBASE/lib
LIBS += -lmeabase
LIBS += -lpthread
DEFINES += VERSION=$$VERSION

mcshardware {
  message("MultiChannel Systems Hardware")
  DEFINES += MCSHARDWARE
}

nihardware {
  message("National Instruments PCI6259 Hardware")
  DEFINES += NIHARDWARE
}

ueihardware {
  message("United Electronic Industries PowerDAQ Hardware")
  DEFINES += PDHARDWARE
}

ultrafast {
  message("Ultrafast")
  DEFINES += MEA_ULTRAFAST
}
