#include <iostream>
#include <map>

#include "../include/Camera.h"
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

std::map<std::string, int> resolutions = {
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

  CameraConfig config = {
      .aspect_ratio      = 16.0 / 9.0,
      .image_width       = resolutions["qHD"],
      .samples_per_pixel = 20,
  };

  Camera cam(config);

  // world

  HittableList world;
  world.add(std::make_shared<Sphere>(cPoint3{0, 0, -1}, 0.5));
  world.add(std::make_shared<Sphere>(cPoint3(0, -100.5, -1), 100));

  // render

  cam.render(world);
}
