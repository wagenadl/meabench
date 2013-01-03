# spikesrv/spikesrv.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = spikesrv
include(../common/meabench.pri)

HEADERS = \
    AdaptiveThreshold.H  Defs.H     IsClean.H     SD_BandFlt.H  SD_SNEO.H \
    BlackHoleSrv.H       Exclude.H  SD_AdaFlt.H   SD_Butter.H   SpikeDets.H \
    Butterworth.H        Filters.H  SD_AdaPerc.H  SD_LimAda.H

SOURCES = \
    AdaptiveThreshold.C  SD_AdaPerc_Detect.C  \
    SD_Butter_Detect.C  SD_SNEO_Detect.C \
    SD_AdaFlt.C          SD_BandFlt.C         SD_LimAda.C         spikedet.C \
    SD_AdaFlt_Detect.C   SD_BandFlt_Detect.C  SD_LimAda_Detect.C  SpikeDets.C \
    SD_AdaPerc.C         SD_Butter.C          SD_SNEO.C
