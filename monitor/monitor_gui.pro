# monitor/monitor_gui.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = monitor
include(../gui/gui.pri)

HEADERS =  Monitor.H  MonTable.H  Progs.H  

SOURCES = monitormain.C  Progs.C  Monitor.C  MonTable.C   

FORMS = Monitor.ui
