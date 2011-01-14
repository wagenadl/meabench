# scope/Makefile.am: part of meabench, an MEA recording and analysis tool
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


bin_PROGRAMS=scope
noinst_HEADERS = Defs.H FreezeInfo.H QMultiScope.H QScope.H Scope.H \
	scrolltest.H Scope.ui Scope.ui.h
scope_SOURCES = scope.C FreezeInfo.C Scope.C QScope.C QMultiScope.C
nodist_scope_SOURCES =	$(BUILT_SOURCES)
BUILT_SOURCES = ScopeD.C ScopeD.H ScopeD.moc.C Scope.moc.C \
	FreezeInfo.moc.C QScope.moc.C QMultiScope.moc.C
CLEANFILES = $(BUILT_SOURCES)
include $(top_srcdir)/Makefile.gui

overwrite: ScopeI.C ScopeI.H
	cp ScopeI.C Scope.C
	cp ScopeI.H Scope.H
