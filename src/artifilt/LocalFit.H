/* artifilt/LocalFit.H: part of meabench, an MEA recording and analysis tool
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

// LocalFit.H

#ifndef LOCALFIT_H

#define LOCALFIT_H

#include <base/Error.H>

/* Notice: BIG WARNING: For large tau, the sums X3 and X1 become very
   large after multiplication with T4 and T6, especially if the signal
   isn't centered at zero initially. This is problematic from tau>=55 if
   the baseline is as large as 4096, or for tau>=57 if the baseline is 2048.
 * Bottom line: *ALWAYS* remove baseline before passing data in.
 * Perhaps the real solution is to re-do the math with X0 subtracted from
   X1..X3.
*/

// following required for Target only, apart from timeref_t and raw_t
#include <common/Types.H>
//#include <rawsrv/Defs.H>
#include <base/CyclBuf.H>

#ifndef LF_TARGET
class LF_Target {
public:
  LF_Target(int hw0, CyclBuf<Sample> &data0): hw(hw0), data(data0) { }
  raw_t const &operator[](timeref_t dt) const { return data[dt][hw]; }
  raw_t &operator[](timeref_t dt) { return data[dt][hw]; }
public: // read only !
  int hw;
private:
  CyclBuf<Sample> &data;
};

typedef LF_Target LF_Source;
typedef LF_Target LF_Dest;

#endif

class LocalFit {
public:
  enum State { OK, PEGGING, PEGGED, TOOPOOR, DEPEGGING, FORCEPEG, BLANKDEPEG };
  /* State variables kept in each state:

    	 var\state OK PEGGING PEGGED TOOPOOR DEPEGGING FORCEPEG BLANKDEPEG
    		   -- ------- ------ ------- --------- -------- ----------
    	 t_stream   y    y      y      y        y         y         y     
    	 t_0        *    y      *      y        y         +         y     
    	 X_0..2     y    y      n      y        y         n         y     
    	 X_3        n    y      n      y        y         n         y     
    	 alpha_0..3 n    y      n      y        y         n         y     
         toopoorcnt n    n      n      y        n         n         n     
	 
     *: t_0 is implicitly equal to t_stream and not kept
     +: t_0 is used to mark end of forced peg
  */
  static const raw_t RAIL1=0;
  static const raw_t RAIL2=4095;
  static const timeref_t TCHI2=15; // samples
  static const timeref_t BLANKDEP=5; // samples
  static const timeref_t AHEAD=5; // samples
  static const timeref_t TOOPOORCNT=5;
  typedef long long int_t;
  typedef double real_t;
public: 
  LocalFit(LF_Source const &source, LF_Dest &dest,
	   timeref_t t_start, raw_t threshold, timeref_t tau,
	   timeref_t t_blankdepeg=BLANKDEP, timeref_t t_ahead=AHEAD,
	   timeref_t t_chi2=TCHI2);
  void reset(timeref_t t_start);
  void setrail(raw_t r1, raw_t r2) { rail1=r1; rail2=r2; }
  timeref_t process(timeref_t t_limit);
  timeref_t forcepeg(timeref_t t_from, timeref_t t_to);
  void enableEvents(class EventReporter *rep) { reporter=rep; }
private:
  void init_T();
  void calc_X012(); // at t0
  void calc_X3(); // at t0
  void update_X0123(); // for t0, from t0-1
  inline void update_X012(); // for t_stream, from t_stream-1 (!)
  void calc_alpha0123(); // from X0123
  inline void calc_alpha0(); // from X02
  State statemachine(timeref_t t_limit, State s);
  bool ispegged(raw_t value) { return value==rail1 || value==rail2; }
private:
  // external world communication
  LF_Source const &source;
  LF_Dest &dest;
private:
  // externally imposed constants
  raw_t y_threshold;
  int tau;
  int t_blankdepeg;
  int t_ahead;
  int t_chi2;
  raw_t rail1, rail2;
private:
  // self computed constants
  int_t tau_plus_1;
  int_t tau_plus_1_squared;
  int_t tau_plus_1_cubed;
  int_t minus_tau;
  int_t minus_tau_squared;
  int_t minus_tau_cubed;
  int_t T0, T2, T4, T6;
  real_t my_thresh;
private:
  // state variables
  State state;
  timeref_t t_stream, t0;
  int_t X0, X1, X2, X3;
  real_t alpha0, alpha1, alpha2, alpha3;
  int toopoorcnt;
  bool negv;
  timeref_t t_peg;
  class EventReporter *reporter;
#if TEST
  timeref_t t_depeg;
  timeref_t t_check;
#endif
public:
  // debug
  void report();
  void inirep();
};

#endif
