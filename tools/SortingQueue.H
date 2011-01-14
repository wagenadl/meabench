/* tools/SortingQueue.H: part of meabench, an MEA recording and analysis tool
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

// SortingQueue.H

#ifndef SORTINGQUEUE_H

#define SORTINGQUEUE_H

#include <set>

template <class Value, class Compare=less<Value> > class SortingQueue:
  public std::set<Value,Compare> {
public:
  void enqueue(Value const &v) { insert(std::set<Value,Compare>::end(), v); }
  bool nonempty() { return !std::set<Value,Compare>::empty(); }
  Value const &first() const { return *std::set<Value,Compare>::begin(); }
  void shift() { erase(std::set<Value,Compare>::begin()); }
};

#endif
