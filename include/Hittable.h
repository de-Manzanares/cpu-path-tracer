#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct HitRecord {
  Point3 p;
  Vec3   normal;
  num_t  t;
  bool   front_face;

  void set_face_normal(cRay &r, cVec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal     = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(cRay &r, cnum_t ray_tmin, cnum_t ray_tmax,
                   HitRecord &rec) const = 0;
};

#endif
