#ifndef VEC3_H
#define VEC3_H

#include <cmath>

using num_t  = double;
using cnum_t = const num_t;

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

inline Vec3 operator*(cnum_t t, cVec3 &v) {
  return Vec3{t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vec3 operator*(cVec3 &v, cnum_t t) { return t * v; }

inline Vec3 operator/(cVec3 &v, cnum_t t) { return 1 / t * v; }

inline num_t dot(cVec3 &u, cVec3 &v) {
  return (u.e[0] * v.e[0]) + (u.e[1] * v.e[1]) + (u.e[2] * v.e[2]);
}

inline Vec3 cross(cVec3 &u, cVec3 &v) {
  return Vec3{(u.e[1] * v.e[2]) - (u.e[2] * v.e[1]),
              (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]),
              (u.e[0] * v.e[1]) - (u.e[1] * v.e[0])};
}

inline Vec3 unit_vector(cVec3 &v) { return v / v.length(); }

#endif
