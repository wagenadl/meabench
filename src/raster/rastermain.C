/* raster/raster.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// raster.C

#include "Raster.H"
#include <common/MEAB.H>
#include <qapplication.h>
#include <base/Error.H>
#include <stdio.h>
#include <base/dbx.H>

int main(int argc, char **argv) {
  MEAB::announce("raster");
#ifdef TEST
  switchdbx(1);
#endif
  try {
    QApplication qapp(argc,argv);
    Raster raster;
    raster.show();
    qapp.exec();
    return 0;
  } catch (Error const &e) {
    dbx("Top level exception");
    e.report();
    fprintf(stderr,"Unexpected exception at top level - quitting\n");
    return 1;
  } catch (...) {
    fprintf(stderr,"Unknown exception\n");
    fprintf(stderr,"Unexpected exception at top level - quitting\n");
    return 2;
  }
}

