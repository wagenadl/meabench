/* spikesrv/Butterworth.H: part of meabench, an MEA recording and analysis tool
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

// Butterworth.H

#ifndef BUTTERWORTH_H

#define BUTTERWORTH_H

class Butterworth {
private:
  // const int ORDER = 3;
  // const double FREQ0 = 400 Hz;
  /* The following coefficients were obtained from matlab 6 using:
     >> [ b a] = butter(3,400/12500,'high');
     >> save coeffs b a -ascii -double
  */
  // 100 Hz (200 Hz commented out)
#define Butterworth_b0  ( 9.7517981163475431e-01)
#define Butterworth_b1  (-2.9255394349042629e+00)
#define Butterworth_b2  ( 2.9255394349042629e+00)
#define Butterworth_b3  (-9.7517981163475431e-01)
#define Butterworth_a0  ( 1.0000000000000000e+00)
#define Butterworth_a1  (-2.9497358397063476e+00)
#define Butterworth_a2  ( 2.9007269883554376e+00)
#define Butterworth_a3  (-9.5097566501624897e-01)
//  static const double b0 =    9.7517981163475431e-01;//  9.5097188792340925e-01;
//  static const double b1 =   -2.9255394349042629e+00;// -2.8529156637702275e+00;
//  static const double b2 =    2.9255394349042629e+00;//  2.8529156637702275e+00;
//  static const double b3 =   -9.7517981163475431e-01;// -9.5097188792340925e-01;
//			a0 =    1.0000000000000000e+00;
//  static const double a1 =   -2.9497358397063476e+00;// -2.8994795946118641e+00;
//  static const double a2 =    2.9007269883554376e+00;//  2.8039479773830003e+00;
//  static const double a3 =   -9.5097566501624897e-01;// -9.0434753139240920e-01;
public:
  Butterworth(double x0=0): x1(x0),x2(x0),x3(x0),y1(0),y2(0),y3(0) { }
  double operator()(double x) {
    //    double y = b0*x + b1*x1 + b2*x2 + b3*x3 - a1*y1 - a2*y2 - a3*y3;
    double y = Butterworth_b0*x + Butterworth_b1*x1 + Butterworth_b2*x2 + Butterworth_b3*x3
      - Butterworth_a1*y1 - Butterworth_a2*y2 - Butterworth_a3*y3;
    x3=x2; x2=x1; x1=x;
    y3=y2; y2=y1; y1=y;
    return y;
  }
  double operator()() const { return y1; }
private:
  double x1, x2, x3;
  double y1, y2, y3;
};

#endif
