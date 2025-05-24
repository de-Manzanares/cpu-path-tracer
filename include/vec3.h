#ifndef VEC3_H
#define VEC3_H

#include <cmath>

using num_t  = double;
using cnum_t = const num_t;

class vec3 {
public:
  num_t e[3];

  vec3() : e{0, 0, 0} {}
  vec3(cnum_t e0, cnum_t e1, cnum_t e2) : e{e0, e1, e2} {}

  num_t x() const { return e[0]; }
  num_t y() const { return e[1]; }
  num_t z() const { return e[2]; }

  vec3 operator-() const & { return vec3(-e[0], -e[1], -e[2]); }

  vec3 operator-() && {
    e[0] = -e[0];
    e[1] = -e[1];
    e[2] = -e[2];
    return std::move(*this);
  }

  num_t operator[](const int i) const { return e[i]; }
  num_t &operator[](const int i) { return e[i]; }

  vec3 &operator+=(const vec3 &t) {
    e[0] += t[0];
    e[1] += t[1];
    e[2] += t[2];
    return *this;
  }

  vec3 &operator*=(cnum_t t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(cnum_t t) { return *this *= 1 / t; }

  num_t length() const { return std::sqrt(length_squared()); }

  num_t length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
};

using point3 = vec3;

inline std::ostream &operator<<(std::ostream &os, const vec3 &v) {
  return os << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3{u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3{u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3{u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline vec3 operator*(cnum_t t, const vec3 &v) {
  return vec3{t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline vec3 operator*(const vec3 &v, cnum_t t) { return t * v; }

inline vec3 operator/(const vec3 &v, cnum_t t) { return 1 / t * v; }

inline num_t dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3{u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

#endif