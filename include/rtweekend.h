#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <random>

using num_t  = double;
using cnum_t = const num_t;

constexpr num_t infty = std::numeric_limits<num_t>::infinity();
constexpr num_t pi    = 3.141592653589793;

inline num_t degrees_to_radians(cnum_t degrees) { return degrees * pi / 180.0; }

/**
 * @param low_b lower bound
 * @param up_b upper bound
 * @return random integer in range [low_b, up_b)
 * @note rand_engine is thread_local to avoid constructing a new engine each
 * time this function is called
 */
template <typename T = num_t, typename = std::enable_if_t<std::is_floating_point_v<T>>>
auto random_num_t(const T low_b = 0, const T up_b = 1) -> T {
  thread_local std::ranlux48_base rand_engine{std::random_device{}()};

  auto dist = std::uniform_real_distribution<T>{static_cast<T>(low_b), static_cast<T>(up_b)};

  return dist(rand_engine);
}

#endif
