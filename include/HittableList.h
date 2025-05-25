#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

struct HittableList : Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() = default;
  HittableList(const std::shared_ptr<Hittable> &object) { add(object); }

  void add(const std::shared_ptr<Hittable> &object) {
    objects.push_back(object);
  }

  void clear() { objects.clear(); }

  bool hit(cRay &r, cInterval ray_t, HitRecord &rec) const override {
    HitRecord tmp_rec;
    bool      hit_anything   = false;
    num_t     closest_so_far = ray_t._max;

    for (const auto &object : objects) {
      if (object->hit(r, cInterval{ray_t._min, closest_so_far}, tmp_rec)) {
        hit_anything   = true;
        closest_so_far = tmp_rec.t;
        rec            = tmp_rec;
      }
    }
    return hit_anything;
  }
};

#endif
