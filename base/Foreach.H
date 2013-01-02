/* base/Foreach.H: part of meabench, an MEA recording and analysis tool
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

// Foreach.H

#ifndef FOREACH_H

#define FOREACH_H

template <class I, class D> void mforeach(I const &b, I const &e,
					 void (D::*foo)()) {
  for (I i=b; i!=e; ++i) {
    D *d=dynamic_cast<D*>(*i);
    if (d)
      (d->*foo)();
  }
}

////template <class I, class D, class A> void foreach(I const &b, I const &e,
////						  void (D::*foo)(A const &),
////						  A const &a0) {
////  for (I i=b; i!=e; ++i) {
////    D *d=dynamic_cast<D*>(*i);
////    if (d)
////	(d->*foo)(a0);
////  }
////}

template <class I, class D, class A> void mforeach(I const &b, I const &e,
						   void (D::*foo)(A &),
						   A &a0) {
  for (I i=b; i!=e; ++i) {
    D *d=dynamic_cast<D*>(*i);
    if (d)
	(d->*foo)(a0);
  }
}

template <class I, class D, class A> void mforeach(I const &b, I const &e,
						  void (D::*foo)(A),
						  A a0) {
  for (I i=b; i!=e; ++i) {
    D *d=dynamic_cast<D*>(*i);
    if (d)
	(d->*foo)(a0);
  }
}

//template <class I, class D, class A, class B>
//  void foreach(I const &b, I const &e,
//		   void (D::*foo)(A const &, B const &),
//		   A const &a0, B const &b0) {
//  for (I i=b; i!=e; ++i) {
//    D *d=dynamic_cast<D*>(*i);
//    if (d)
//	  (d->*foo)(a0,b0);
//  }
//}


//template <class I, class D, class A, class B>
//  void foreach(I const &b, I const &e,
//		   void (D::*foo)(A &, B &),
//		   A &a0, B &b0) {
//  for (I i=b; i!=e; ++i) {
//    D *d=dynamic_cast<D*>(*i);
//    if (d)
//	  (d->*foo)(a0,b0);
//  }
//}

//template <class I, class D, class A, class B>
//  void foreach(I const &b, I const &e,
//		 void (D::*foo)(A,B),
//		 A a0, B b0) {
//  for (I i=b; i!=e; ++i) {
//    D *d=dynamic_cast<D*>(*i);
//    if (e)
//	  (d->*foo)(a0,b0);
//  }
//}

#endif
