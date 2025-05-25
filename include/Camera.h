#ifndef CAMERA_H
#define CAMERA_H

#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "rtweekend.h"

#include <iostream>

class Camera {
public:
  Camera(cnum_t aspect_ratio = 16.0 / 9.0, const int image_width = 960) {

    // image

    _aspect_ratio = aspect_ratio;
    _image_width  = image_width;

    _image_height = _image_width / _aspect_ratio < 1
                        ? 1
                        : static_cast<int>(_image_width / _aspect_ratio);

    // camera

    _center                         = Point3{0, 0, 0};
    constexpr num_t focal_length    = 1.0;
    constexpr num_t viewport_height = 2.0;
    cnum_t          viewport_width =
        viewport_height * (static_cast<num_t>(_image_width) / _image_height);

    // horizontal and vertical viewport edges

    const auto viewport_u = Vec3{viewport_width, 0, 0};
    const auto viewport_v = Vec3{0, -viewport_height, 0};

    // pixel deltas

    _pixel_delta_u = viewport_u / _image_width;
    _pixel_delta_v = viewport_v / _image_height;

    // location of upper left pixel

    const auto viewport_upper_left =
        _center - Vec3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
    _pixel00_loc =
        viewport_upper_left + 0.5 * (_pixel_delta_u + _pixel_delta_v);
  }

  void render(cHittableList &world) const {
    std::cout << "P3\n" << _image_width << " " << _image_height << "\n255\n";
    for (int j = 0; j < _image_height; j++) {
      std::clog << "\rScanlines remaining: " << (_image_height - j)
                << std::flush;
      for (int i = 0; i < _image_width; i++) {
        auto pixel_center =
            _pixel00_loc + (i * _pixel_delta_u) + (j * _pixel_delta_v);
        auto  ray_direction = pixel_center - _center;
        Color pixel_color   = ray_color(Ray{_center, ray_direction}, world);
        write_color(std::cout, pixel_color);
      }
    }
    std::clog << "\rDone.                                                   \n";
  }

private:
  num_t  _aspect_ratio;  // Ratio of image width over height
  int    _image_width;   // Rendered image width in pixel count
  int    _image_height;  // Rendered image height
  Point3 _center;        // Camera center
  Point3 _pixel00_loc;   // Location of pixel 0, 0
  Vec3   _pixel_delta_u; // Offset to pixel to the right
  Vec3   _pixel_delta_v; // Offset to pixel below

  static Color ray_color(cRay &r, cHittableList &world) {
    if (HitRecord rec; world.hit(r, cInterval{0, infty}, rec)) {
      return 0.5 * (rec.N + cVec3{1, 1, 1});
    }
    cVec3      unit_direction = unit_vector(r.direction());
    const auto a              = 0.5 * (unit_direction.y() + 1.0);
    // blue to orange linear interpolation -- horizontal and vertical gradient
    return (1.0 - a) * Color{253.0 / 255.0, 94.0 / 255.0, 83.0 / 255.0} +
           a * Color{55.0 / 255.0, 198.0 / 255.0, 1.0};
  }
};

#endif
