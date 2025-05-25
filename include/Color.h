#ifndef COLOR_H
#define COLOR_H

#include "Interval.h"
#include "Vec3.h"

#include <iostream>

using Color = Vec3;

inline num_t linear_to_gamma(cnum_t linear_component) {
  if (linear_component > 0) {
    return std::sqrt(linear_component);
  }
  return 0;
}

inline void write_color(std::ostream &os, const Color &pixel_color) {
  const auto r = linear_to_gamma(pixel_color.x());
  const auto g = linear_to_gamma(pixel_color.y());
  const auto b = linear_to_gamma(pixel_color.z());

  static cInterval intensity{0.000, 0.999};

  // scale to [0,255]

  const int rbyte = static_cast<int>(256 * intensity.clamp(r));
  const int gbyte = static_cast<int>(256 * intensity.clamp(g));
  const int bbyte = static_cast<int>(256 * intensity.clamp(b));

  os << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
