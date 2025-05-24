#include <iostream>

#include "../include/color.h"
#include "../include/vec3.h"

int main() {
  constexpr int image_height = 256;
  constexpr int image_width  = 256;

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int i = 0; i < image_height; i++) {
    std::clog << "\rScanlines remaining: " << (image_height - i) << std::flush;
    for (int j = 0; j < image_width; j++) {
      const auto pixel_color = color{
          static_cast<num_t>(j) / (image_width - 1),
          static_cast<num_t>(i) / (image_height - 1),
          0,
      };
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                                                     \n";
}
