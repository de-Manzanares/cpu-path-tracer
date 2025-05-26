#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "Hittable.h"
#include "Ray.h"

class Material {
public:
  virtual ~Material() = default;

  virtual bool scatter(
      cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
  ) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const Color &albedo) : _albedo{albedo} {}

  bool scatter(
      cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
  ) const override {
    Vec3 scatter_direction = rec.N + random_unit_vector();
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.N;
    }
    scattered   = Ray{rec.p, scatter_direction};
    attenuation = _albedo;
    return true;
  }

private:
  Color _albedo;
};

class Metal : public Material {
public:
  Metal(const Color &albedo) : _albedo(albedo) {}

  bool scatter(
      cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
  ) const override {
    cVec3 reflected = reflect(r_in.direction(), rec.N);
    scattered       = Ray(rec.p, reflected);
    attenuation     = _albedo;
    return true;
  }

private:
  Color _albedo;
};

#endif
