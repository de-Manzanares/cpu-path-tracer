#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include <cmath>

class Vec3 {
public:
  num_t e[3];

  Vec3() : e{0, 0, 0} {}
  Vec3(cnum_t e0, cnum_t e1, cnum_t e2) : e{e0, e1, e2} {}

  [[nodiscard]] num_t x() const { return e[0]; }
  [[nodiscard]] num_t y() const { return e[1]; }
  [[nodiscard]] num_t z() const { return e[2]; }

  Vec3 operator-() const & { return Vec3(-e[0], -e[1], -e[2]); }

  Vec3 operator-() && {
    e[0] = -e[0];
    e[1] = -e[1];
    e[2] = -e[2];
    return *this;
  }

  num_t  operator[](const int i) const { return e[i]; }
  num_t &operator[](const int i) { return e[i]; }

  Vec3 &operator+=(const Vec3 &t) {
    e[0] += t[0];
    e[1] += t[1];
    e[2] += t[2];
    return *this;
  }

  Vec3 &operator*=(cnum_t t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3 &operator/=(cnum_t t) { return *this *= 1 / t; }

  [[nodiscard]] num_t length() const { return std::sqrt(length_squared()); }

  [[nodiscard]] num_t length_squared() const {
    return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
  }

  [[nodiscard]] bool near_zero() const {
    constexpr cnum_t s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
  }

  [[nodiscard]] static Vec3 random() {
    return Vec3{random_num_t(), random_num_t(), random_num_t()};
  }

  [[nodiscard]] static Vec3 random(cnum_t min, cnum_t max) {
    return Vec3{random_num_t(min, max), random_num_t(min, max), random_num_t(min, max)};
  }
};

using cVec3   = const Vec3;
using Point3  = Vec3;
using cPoint3 = const Point3;

inline std::ostream &operator<<(std::ostream &os, cVec3 &v) {
  return os << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(cVec3 &u, cVec3 &v) {
  return Vec3{u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vec3 operator-(cVec3 &u, cVec3 &v) {
  return Vec3{u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vec3 operator*(cVec3 &u, cVec3 &v) {
  return Vec3{u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline Vec3 operator*(cnum_t t, cVec3 &v) { return Vec3{t * v.e[0], t * v.e[1], t * v.e[2]}; }

inline Vec3 operator*(cVec3 &v, cnum_t t) { return t * v; }

inline Vec3 operator/(cVec3 &v, cnum_t t) { return 1 / t * v; }

inline num_t dot(cVec3 &u, cVec3 &v) {
  return (u.e[0] * v.e[0]) + (u.e[1] * v.e[1]) + (u.e[2] * v.e[2]);
}

inline Vec3 cross(cVec3 &u, cVec3 &v) {
  return Vec3{
      (u.e[1] * v.e[2]) - (u.e[2] * v.e[1]), (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]),
      (u.e[0] * v.e[1]) - (u.e[1] * v.e[0])
  };
}

inline Vec3 unit_vector(cVec3 &v) { return v / v.length(); }

inline Vec3 random_unit_vector() {
  while (true) {
    auto p     = Vec3::random(-1, 1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1) {
      return p / sqrt(lensq);
    }
  }
}

inline Vec3 random_on_hemisphere(cVec3 &normal) {
  Vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) { // same hemisphere as normal
    return on_unit_sphere;
  }
  return -std::move(on_unit_sphere);
}

inline Vec3 reflect(cVec3 &v, cVec3 &N) { return v - 2 * dot(v, N) * N; }

#endif
