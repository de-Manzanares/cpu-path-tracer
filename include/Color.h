#ifndef COLOR_H
#define COLOR_H

#include "Interval.h"
#include "Vec3.h"

#include <iostream>

using Color = Vec3;

inline void write_color(std::ostream &os, const Color &pixel_color) {
  const auto r = pixel_color.x();
  const auto g = pixel_color.y();
  const auto b = pixel_color.z();

  static cInterval intensity{0.000, 0.999};

  // scale to [0,255]

  const int rbyte = static_cast<int>(256 * intensity.clamp(r));
  const int gbyte = static_cast<int>(256 * intensity.clamp(g));
  const int bbyte = static_cast<int>(256 * intensity.clamp(b));

  os << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
