#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "Hittable.h"
#include "Ray.h"

class Material {
public:
  virtual ~Material() = default;

  virtual bool scatter(cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const Color &albedo) : _albedo{albedo} {}

  bool
  scatter(cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override {
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
  Metal(const Color &albedo, cnum_t fuzz) : _albedo(albedo), _fuzz{fuzz < 1 ? fuzz : 1} {}

  bool
  scatter(cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override {
    Vec3 reflected = reflect(r_in.direction(), rec.N);
    reflected      = unit_vector(reflected) + (_fuzz * random_unit_vector());
    scattered      = Ray(rec.p, reflected);
    attenuation    = _albedo;
    return (dot(scattered.direction(), rec.N) > 0);
  }

private:
  Color _albedo;
  num_t _fuzz;
};

class Dielectric : public Material {
public:
  Dielectric(cnum_t ri) : _ri{ri} {}

  bool
  scatter(cRay &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override {
    attenuation               = Color{1.0, 1.0, 1.0};
    cnum_t     ri             = rec.front_face ? (1.0 / _ri) : _ri; // 1.0 is ri of air
    cVec3      unit_direction = unit_vector(r_in.direction());
    cnum_t     cos_theta      = std::fmin(dot(-unit_direction, rec.N), 1.0);
    cnum_t     sin_theta      = std::sqrt(1.0 - (cos_theta * cos_theta));
    const bool cannot_refract = ri * sin_theta > 1.0;
    Vec3       direction;

    // compare reflectance to random n to coin flip on reflection or refraction
    // instead of always spawning both rays
    if (cannot_refract || reflectance(cos_theta, ri) > random_num_t()) {
      direction = reflect(unit_direction, rec.N);
    } else {
      direction = refract(unit_direction, rec.N, ri);
    }

    scattered = Ray{rec.p, direction};
    return true;
  }

private:
  num_t _ri; // refractive index

  static num_t reflectance(cnum_t cosine, cnum_t refraction_index) {
    // Schlick's approximation
    num_t r0 = (1 - refraction_index) / (1 + refraction_index);
    r0       = r0 * r0;
    return r0 + ((1 - r0) * std::pow((1 - cosine), 5));
  }
};

#endif
