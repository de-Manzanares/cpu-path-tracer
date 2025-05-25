#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>

using num_t  = double;
using cnum_t = const num_t;

constexpr num_t infty = std::numeric_limits<num_t>::infinity();
constexpr num_t pi    = 3.141592653589793;

inline num_t degrees_to_radians(cnum_t degrees) { return degrees * pi / 180.0; }

#endif
