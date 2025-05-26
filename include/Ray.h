#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
  Ray() {}

  Ray(cPoint3 &origin, cVec3 &direction) : _origin{origin}, _direction{direction} {}

  [[nodiscard]] cPoint3 &origin() const { return _origin; }
  [[nodiscard]] cVec3   &direction() const { return _direction; }

  [[nodiscard]] Point3 at(cnum_t t) const { return _origin + t * _direction; }

private:
  Point3 _origin;
  Vec3   _direction;
};

using cRay = const Ray;

#endif
