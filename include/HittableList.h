#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

struct HittableList : Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList(const std::shared_ptr<Hittable> &object) { add(object); }

  void add(const std::shared_ptr<Hittable> &object) {
    objects.push_back(object);
  }

  void clear() { objects.clear(); }

  bool hit(cRay &r, cnum_t ray_tmin, cnum_t ray_tmax,
           HitRecord &rec) const override {
    HitRecord tmp_rec;
    bool      hit_anything   = false;
    num_t     closest_so_far = ray_tmax;

    for (const auto &object : objects) {
      if (object->hit(r, ray_tmin, ray_tmax, tmp_rec)) {
        hit_anything   = true;
        closest_so_far = tmp_rec.t;
        rec            = tmp_rec;
      }
    }
    return hit_anything;
  }
};

#endif
