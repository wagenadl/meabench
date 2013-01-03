# raster/multiraster.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = raster
include(../gui/gui.pri)

HEADERS = Raster.H \
    QRaster.H QMultiRaster.H MultiRasterBase.H SpikeBuffer.H \
    QRDispInfo.H
    
SOURCES = rastermain.C \
    Raster.C \
    QRaster.C QMultiRaster.C MultiRasterBase.C SpikeBuffer.C

FORMS = Raster.ui
