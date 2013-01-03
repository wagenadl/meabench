# replay/replay.pro - part of MEABench, an MEA replaying and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = replay
include(../common/meabench.pri)

HEADERS = Defs.H  RawReplay.H  ReplayBase.H  SpikeReplay.H  TriggerFile.H
SOURCES = RawReplay.C  ReplayBase.C  replay.C  SpikeReplay.C  TriggerFile.C

