#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public Hittable {
public:
  sphere(cpoint3 &center, cnum_t radius)
      : _center(center), _radius(std::fmax(0, radius)) {}

  bool hit(const ray &r, cnum_t ray_tmin, cnum_t ray_tmax,
           Hit_Record &rec) const override {
    cvec3  oc = _center - r.origin();
    cnum_t a  = r.direction().length_squared();
    cnum_t h  = dot(r.direction(), oc);
    cnum_t c  = oc.length_squared() - _radius * _radius;

    cnum_t discriminant = h * h - a * c;
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (h + sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root) {
        return false;
      }
    }

    rec.t      = root;
    rec.p      = r.at(rec.t);
    rec.normal = (rec.p - _center) / _radius;

    return true;
  }

private:
  point3 _center;
  num_t  _radius;
};

#endif
