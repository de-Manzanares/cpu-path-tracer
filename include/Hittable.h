#ifndef HITTABLE_H
#define HITTABLE_H

#include "Interval.h"
#include "Ray.h"

struct HitRecord {
  Point3 p;
  Vec3   N;
  num_t  t;
  bool   front_face;

  void set_face_normal(cRay &r, cVec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    N          = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(cRay &r, cInterval ray_t, HitRecord &rec) const = 0;
};

#endif
