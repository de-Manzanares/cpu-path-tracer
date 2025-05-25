#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct Hit_Record {
  point3 p;
  vec3   normal;
  num_t  t;
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(const ray &r, cnum_t ray_tmin, cnum_t ray_tmax,
                   Hit_Record &rec) const = 0;
};

#endif
