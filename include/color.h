#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

inline void write_color(std::ostream &os, const color &pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // scale to [0,255]
  const int rbyte = static_cast<int>(255.999 * r);
  const int gbyte = static_cast<int>(255.999 * g);
  const int bbyte = static_cast<int>(255.999 * b);

  os << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
