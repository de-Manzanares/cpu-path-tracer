#ifndef CAMERA_H
#define CAMERA_H

#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "rtweekend.h"

#include <iostream>

struct CameraConfig {
  num_t  aspect_ratio;
  int    image_width;
  int    samples_per_pixel;
  int    max_depth;
  num_t  vfov;
  Point3 look_from;
  Point3 look_at;
  Vec3   vup;
};

class Camera {
public:
  Camera(const CameraConfig &config) {

    // image

    _aspect_ratio       = config.aspect_ratio;
    _image_width        = config.image_width;
    _samples_per_pixel  = config.samples_per_pixel;
    _pixel_sample_scale = 1.0 / _samples_per_pixel;
    _max_depth          = config.max_depth;
    _vfov               = config.vfov;
    _camera_center      = config.look_from;
    _look_at            = config.look_at;
    _vup                = config.vup;

    _image_height =
        _image_width / _aspect_ratio < 1 ? 1 : static_cast<int>(_image_width / _aspect_ratio);

    // viewport dimensions

    cnum_t focal_length    = (_look_at - _camera_center).length();
    cnum_t theta           = degrees_to_radians(_vfov);
    cnum_t h               = std::tan(theta / 2);
    cnum_t viewport_height = 2 * h * focal_length;
    cnum_t viewport_width  = viewport_height * (static_cast<num_t>(_image_width) / _image_height);

    // unit basis vectors for camera plane

    _w = unit_vector(_camera_center - _look_at);
    _u = unit_vector(cross(_vup, _w));
    _v = cross(_w, _u);

    // horizontal and vertical viewport edges

    cVec3 viewport_u = viewport_width * _u;
    cVec3 viewport_v = -viewport_height * _v;

    // pixel deltas

    _pixel_delta_u = viewport_u / _image_width;
    _pixel_delta_v = viewport_v / _image_height;

    // location of upper left pixel

    cVec3 viewport_upper_left =
        _camera_center - (focal_length * _w) - (viewport_u / 2) - (viewport_v / 2);
    _pixel00_loc = viewport_upper_left + 0.5 * (_pixel_delta_u + _pixel_delta_v);
  }

  void render(cHittableList &world) const {
    std::cout << "P3\n" << _image_width << " " << _image_height << "\n255\n";
    for (int j = 0; j < _image_height; j++) {
      std::clog << "\rScanlines remaining: " << (_image_height - j) << std::flush;
      if ((_image_height - j) % 100 == 0) {
        std::clog << "\r                                            " << std::flush;
      }
      for (int i = 0; i < _image_width; i++) {
        if (_samples_per_pixel == 1) {
          auto  pixel_center  = _pixel00_loc + (i * _pixel_delta_u) + (j * _pixel_delta_v);
          auto  ray_direction = pixel_center - _camera_center;
          Color pixel_color   = ray_color(Ray{_camera_center, ray_direction}, _max_depth, world);
          write_color(std::cout, pixel_color);
        } else {
          auto pixel_color = Color{0, 0, 0};
          for (int sample = 0; sample < _samples_per_pixel; sample++) {
            Ray r        = get_ray(i, j);
            pixel_color += ray_color(r, _max_depth, world);
          }
          write_color(std::cout, _pixel_sample_scale * pixel_color);
        }
      }
    }
    std::clog << "\rDone.                                                   \n";
  }

private:
  num_t  _aspect_ratio;       // Ratio of image width over height
  int    _image_width;        // Rendered image width in pixel count
  int    _image_height;       // Rendered image height
  int    _samples_per_pixel;  // Count of random samples for each pixel
  int    _max_depth;          // Limit the number of reflections per ray
  num_t  _vfov;               // Veritcal fov
  Point3 _camera_center;      // Camera center
  Point3 _look_at;            // Look toward here
  Vec3   _vup;                // Defines "up"
  Vec3   _u;                  // camera view unit basis vector right
  Vec3   _v;                  // camera view unit basis vector up
  Vec3   _w;                  // camera view unit basis vector view direction
  num_t  _pixel_sample_scale; // Color scale factor for a sum of pixel samples
  Point3 _pixel00_loc;        // Location of pixel 0, 0
  Vec3   _pixel_delta_u;      // Offset to pixel to the right
  Vec3   _pixel_delta_v;      // Offset to pixel below

  static Color ray_color(cRay &r, const int depth, cHittableList &world) {
    if (depth <= 0) {
      return Color{0, 0, 0};
    }

    if (HitRecord rec; world.hit(r, cInterval{0.001, infty}, rec)) {
      // return 0.5 * (rec.N + cVec3{1, 1, 1}); // normal vec color coding
      // grey
      //  cVec3 direction = rec.N + random_unit_vector();
      // return 0.5 * ray_color(Ray{rec.p, direction}, depth - 1, world);
      Ray   scattered;
      Color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return Color{0, 0, 0};
    }
    cVec3      unit_direction = unit_vector(r.direction());
    const auto a              = 0.5 * (unit_direction.y() + 1.0);
    // blue to orange linear interpolation -- horizontal and vertical gradient
    return (1.0 - a) * Color{253.0 / 255.0, 94.0 / 255.0, 83.0 / 255.0} +
           a * Color{55.0 / 255.0, 198.0 / 255.0, 1.0};
  }

  Ray get_ray(const int i, const int j) const {
    cVec3   offset = sample_square();
    cPoint3 pixel_sample =
        _pixel00_loc + ((i + offset.x()) * _pixel_delta_u) + ((j + offset.y()) * _pixel_delta_v);
    cPoint3 ray_origin    = _camera_center;
    cVec3   ray_direction = pixel_sample - ray_origin;

    return Ray{ray_origin, ray_direction};
  }

  static Vec3 sample_square() { return Vec3{random_num_t() - 0.5, random_num_t() - 0.5, 0}; }
};

#endif
