#include <iostream>

#include "../include/color.h"
#include "../include/ray.h"
#include "../include/vec3.h"

bool hit_sphere(const point3 &center, cnum_t radius, const ray &r) {
  const vec3 oc           = center - r.origin();
  const auto a            = dot(r.direction(), r.direction());
  const auto b            = -2.0 * dot(r.direction(), oc);
  const auto c            = dot(oc, oc) - radius * radius;
  const auto discriminant = b * b - 4 * a * c;
  return (discriminant >= 0);
}

color ray_color(const ray &r) {
  if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
    return color(1, 0, 0);
  }
  const vec3 unit_direction = unit_vector(r.direction());
  const auto a              = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * color{253.0 / 255.0, 94.0 / 255.0, 83.0 / 255.0} +
         a * color{55.0 / 255.0, 198.0 / 255.0, 255.0 / 255.0};
}

int main() {

  // image

  constexpr num_t aspect_ratio = 16.0 / 9.0;
  constexpr num_t image_width  = 2560;

  constexpr num_t image_height =
      image_width / aspect_ratio < 1
          ? 1
          : static_cast<int>(image_width / aspect_ratio);

  // camera

  constexpr num_t focal_length    = 1.0;
  const auto      camera_center   = point3{0, 0, 0};
  constexpr num_t viewport_height = 2.0;
  constexpr num_t viewport_width =
      viewport_height * (static_cast<num_t>(image_width) / image_height);

  // horizontal and vertical viewport edges

  const auto viewport_u = vec3{viewport_width, 0, 0};
  const auto viewport_v = vec3{0, -viewport_height, 0};

  // pixel deltas

  const auto pixel_delta_u = viewport_u / image_width;
  const auto pixel_delta_v = viewport_v / image_height;

  // location of upper left pixel

  const auto viewport_upper_left = camera_center - vec3{0, 0, focal_length} -
                                   viewport_u / 2 - viewport_v / 2;
  const auto pixel00_loc =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // render

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << std::flush;
    for (int i = 0; i < image_width; i++) {
      auto pixel_center =
          pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
      auto  ray_direction = pixel_center - camera_center;
      color pixel_color   = ray_color(ray{camera_center, ray_direction});
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                                                     \n";
}
