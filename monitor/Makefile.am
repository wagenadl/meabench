# monitor/Makefile.am: part of meabench, an MEA recording and analysis tool
# Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


bin_PROGRAMS=monitor meamon climon
noinst_HEADERS = Monitor.H MonTable.H Progs.H Monitor.ui tern.H MeaMon.H
monitor_SOURCES = monitor.C MonTable.C Monitor.C Progs.C 
meamon_SOURCES = meamon.C MeaMon.C tern.C
climon_SOURCES = climon.C tern.C
nodist_meamon_SOURCES = MeaMon.moc.C
nodist_monitor_SOURCES = MonitorD.C MonitorD.moc.C Monitor.moc.C \
	MonTable.moc.C MonitorD.H
BUILT_SOURCES = MonitorD.C MonitorD.moc.C Monitor.moc.C \
	MonTable.moc.C MonitorD.H MeaMon.moc.C
CLEANFILES = $(BUILT_SOURCES)

include $(top_srcdir)/Makefile.gui

overwrite: MonitorI.C MonitorI.H
	cp MonitorI.C Monitor.C
	cp MonitorI.H Monitor.H