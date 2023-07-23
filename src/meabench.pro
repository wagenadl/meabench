# meabench.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = subdirs
SUBDIRS = base
SUBDIRS += 60hz artifilt extractwindow rawsrv record replay rms
SUBDIRS += gui
SUBDIRS += flexraster raster scope spikesound spikesrv
SUBDIRS += neurosock
SUBDIRS += tools
SUBDIRS += utils
CONFIG += ordered
# More to be added in a little bit

include(../config.pri)

QMAKE_EXTRA_TARGETS =  inst
inst.commands = ../buildscripts/install.sh $$PREFIX

