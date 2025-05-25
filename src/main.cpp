#include <iostream>

#include "../include/Color.h"
#include "../include/Ray.h"
#include "../include/Sphere.h"
#include "../include/Vec3.h"

num_t hit_sphere(cPoint3 &center, cnum_t radius, cRay &r) {
  cVec3  oc = center - r.origin();
  cnum_t a  = r.direction().length_squared();
  cnum_t h  = dot(r.direction(), oc);
  cnum_t c  = oc.length_squared() - (radius * radius);

  cnum_t discriminant = (h * h) - (a * c);
  if (discriminant < 0) {
    return -1.0;
  }
  return (h - std::sqrt(discriminant)) / a;
}

Color ray_color(cRay &r) {
  if (cnum_t t = hit_sphere(Point3(0, 0, -1), 0.5, r); t > 0) {
    cVec3 N = r.at(t) - Vec3{0, 0, -1};
    // f(x)=(x+1)/2 maps [-1,1] to [0,1]
    return 0.5 * Color{N.x() + 1, N.y() + 1, N.z() + 1};
  }

  cVec3      unit_direction = unit_vector(r.direction());
  const auto a              = 0.5 * (unit_direction.y() + 1.0);

  // blue to orange linear interpolation -- horizontal and vertical gradient
  return (1.0 - a) * Color{253.0 / 255.0, 94.0 / 255.0, 83.0 / 255.0} +
         a * Color{55.0 / 255.0, 198.0 / 255.0, 255.0 / 255.0};
}

int main() {

  // image

  constexpr num_t aspect_ratio = 16.0 / 9.0;
  constexpr num_t image_width  = 720;

  constexpr num_t image_height =
      image_width / aspect_ratio < 1
          ? 1
          : static_cast<int>(image_width / aspect_ratio);

  // camera

  constexpr num_t focal_length    = 1.0;
  const auto      camera_center   = Point3{0, 0, 0};
  constexpr num_t viewport_height = 2.0;
  constexpr num_t viewport_width =
      viewport_height * (static_cast<num_t>(image_width) / image_height);

  // horizontal and vertical viewport edges

  const auto viewport_u = Vec3{viewport_width, 0, 0};
  const auto viewport_v = Vec3{0, -viewport_height, 0};

  // pixel deltas

  const auto pixel_delta_u = viewport_u / image_width;
  const auto pixel_delta_v = viewport_v / image_height;

  // location of upper left pixel

  const auto viewport_upper_left = camera_center - Vec3{0, 0, focal_length} -
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
      Color pixel_color   = ray_color(Ray{camera_center, ray_direction});
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                                                     \n";
}
