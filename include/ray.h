#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
  ray() {}

  ray(cpoint3 &origin, cvec3 &direction)
      : _origin{origin}, _direction{direction} {}

  cpoint3 &origin() const { return _origin; }
  cvec3   &direction() const { return _direction; }

  point3 at(cnum_t t) const { return _origin + t * _direction; }

private:
  point3 _origin;
  vec3   _direction;
};

#endif
