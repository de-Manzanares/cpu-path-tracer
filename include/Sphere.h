#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
  Sphere(cPoint3 &center, cnum_t radius)
      : _center(center), _radius(std::fmax(0, radius)) {}

  bool hit(cRay &r, cnum_t ray_tmin, cnum_t ray_tmax,
           HitRecord &rec) const override {
    cVec3  oc = _center - r.origin();
    cnum_t a  = r.direction().length_squared();
    cnum_t h  = dot(r.direction(), oc);
    cnum_t c  = oc.length_squared() - _radius * _radius;

    cnum_t discriminant = (h * h) - (a * c);
    if (discriminant < 0) {
      return false;
    }

    const auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (h + sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.N = (rec.p - _center) / _radius;

    cVec3 outward_normal = (rec.p - _center) / _radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }

private:
  Point3 _center;
  num_t  _radius;
};

#endif
