# raster/autoraster.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = autoraster
include(../gui/gui.pri)

HEADERS =  AutoRaster.H \
    QAutoRaster.H QRaster.H QMultiRaster.H MultiRasterBase.H SpikeBuffer.H \
    QRDispInfo.H
    
SOURCES = autorastermain.C \
    AutoRaster.C \
    QAutoRaster.C QRaster.C QMultiRaster.C MultiRasterBase.C SpikeBuffer.C

FORMS = AutoRaster.ui
