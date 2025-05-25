#include <iostream>
#include <map>

#include "../include/Color.h"
#include "../include/HittableList.h"
#include "../include/Interval.h"
#include "../include/Ray.h"
#include "../include/Sphere.h"
#include "../include/Vec3.h"
#include "../include/rtweekend.h"

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

Color ray_color(cRay &r, const HittableList &world) {
  if (HitRecord rec; world.hit(r, cInterval{0, infty}, rec)) {
    return 0.5 * (rec.N + cVec3{1, 1, 1});
  }

  cVec3      unit_direction = unit_vector(r.direction());
  const auto a              = 0.5 * (unit_direction.y() + 1.0);

  // blue to orange linear interpolation -- horizontal and vertical gradient
  return (1.0 - a) * Color{253.0 / 255.0, 94.0 / 255.0, 83.0 / 255.0} +
         a * Color{55.0 / 255.0, 198.0 / 255.0, 255.0 / 255.0};
}

// qHD 		960 	540 	16:9 	0.518 	0.52M9
// HD 		1280 	720 	16:9 	0.922 	0.92M9
// HD+ 		1600 	900 	16:9 	1.440 	1.44M9
// FHD 	   2K 	1920 	1080 	16:9 	2.074 	2.07M9
// (W)QHD 	2560 	1440 	16:9 	3.686 	3.69M9
// QHD+ 	3200 	1800 	16:9 	5.760 	5.76M9
// UHD 	   4K 	3840 	2160 	16:9 	8.294 	8.29M9
//         5K 	5120 	2880 	16:9 	14.746 	14.75M9
// UHD 	   8K 	7680 	4320 	16:9 	33.178 	33.18M9
//         16K 	15360 	8640 	16:9 	132.710 132.71M9

std::map<std::string, num_t> resolutions = {
    {"qHD",  960  }, // quarter
    {"HD",   1280 },
    {"HD+",  1600 },
    {"FHD",  1920 }, // full
    {"2K",   1920 },
    {"QHD",  2560 }, // quad
    {"QHD+", 3200 },
    {"4K",   3840 },
    {"5K",   5120 },
    {"8K",   7680 },
    {"16K",  15360}
};

int main() {

  // image

  constexpr num_t aspect_ratio = 16.0 / 9.0;
  cnum_t          image_width  = resolutions["qHD"];

  cnum_t image_height = image_width / aspect_ratio < 1
                            ? 1
                            : static_cast<int>(image_width / aspect_ratio);

  // world

  HittableList world;
  world.add(std::make_shared<Sphere>(cPoint3{0, 0, -1}, 0.5));
  world.add(std::make_shared<Sphere>(cPoint3(0, -100.5, -1), 100));

  // camera

  constexpr num_t focal_length    = 1.0;
  const auto      camera_center   = Point3{0, 0, 0};
  constexpr num_t viewport_height = 2.0;
  cnum_t          viewport_width =
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
      auto ray_direction = pixel_center - camera_center;

      Color pixel_color = ray_color(Ray{camera_center, ray_direction}, world);
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                                                     \n";
}
