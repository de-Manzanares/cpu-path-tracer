#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

struct Interval {
  num_t _min;
  num_t _max;

  Interval() : _min{infty}, _max{-infty} {}
  Interval(cnum_t min, cnum_t max) : _min{min}, _max{max} {}

  num_t size() const { return _max - _min; }
  bool  contains(cnum_t x) const { return _min <= x && x <= _max; }
  bool  surrounds(cnum_t x) const { return _min < x && x < _max; }

  static const Interval empty;
  static const Interval universe;
};

using cInterval = const Interval;

cInterval Interval::empty    = cInterval{infty, -infty};
cInterval Interval::universe = cInterval{-infty, infty};

#endif
