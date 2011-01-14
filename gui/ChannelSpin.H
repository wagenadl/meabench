/* gui/ChannelSpin.H: part of meabench, an MEA recording and analysis tool
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

// ChannelSpin.H

#ifndef CHANNELSPIN_H

#define CHANNELSPIN_H

#include <qspinbox.h>

class ChannelSpin: public QSpinBox { //:C
  /*:D ChannelSpin is a spin box that displays channel strings from
       11 to 88 (never showing 11, 18, 81 or 88) [MEA60SHAPE].
       These channels strings are mapped to integer channel numbers
       in 0..59 (FIXME: manual change with NCHANS).
  */
  Q_OBJECT
public:
  ChannelSpin(QWidget * parent = 0, const char * name = 0 );
  virtual QString mapValueToText ( int v );
  virtual int mapTextToValue ( bool * ok );
  /*:D Mapping is 0 -> "12", 1->"13", ... 5->"17", 6->"21", ...,59->"87",
     . 60->"A1", 61->"A2", 62->"A3", 63->"D?".
  */
  virtual void polish();
  int hwValue(); // returns hardware channel nr associated with current value
public slots:
  void setHwValue(int); // set value by hardware channel nr
};

#endif
