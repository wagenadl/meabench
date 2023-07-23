# scope/scope.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
TARGET = scope
include(../gui/gui.pri)

HEADERS = Defs.H  FreezeInfo.H  QMultiScope.H  QScope.H  Scope.H    
SOURCES = FreezeInfo.C  QMultiScope.C  QScope.C  Scope.C  scopemain.C

FORMS = Scope.ui
