# flexraster/flexraster.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = flexraster
QT += widgets
include(../gui/gui.pri)

HEADERS = \
    ContPanel.H    ControlPanel.H  LinePanel.H   RD_Rec.H     SimpleRaster.H \
    ContRaster.H   GridPanel.H     OnePanel.H    RD_Stim.H    Storage.H \
    ControlInfo.H  identitrig.H    RasterData.H  Scrollbar.H

SOURCES = \
    ContPanel.C     flexraster.C  OnePanel.C    RD_Stim.C       Storage.C \
    ContRaster.C    GridPanel.C   RasterData.C  Scrollbar.C \
    ControlPanel.C  LinePanel.C   RD_Rec.C      SimpleRaster.C 

FORMS = controlpanel.ui
