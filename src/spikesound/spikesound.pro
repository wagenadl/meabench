# spikesound/spikesound.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = subdirs
SUBDIRS = spikesound_gui spk2wav spk2raw

spikesound_gui.file = spikesound_gui.pro
spk2wav.file = spk2wav.pro
spk2raw.file = spk2raw.pro
