# 60hz/60hz.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = subdirs

SUBDIRS = filter60hz posthoc60hz
filter60hz.file = filter60hz.pro
posthoc60hz.file = posthoc60hz.pro
