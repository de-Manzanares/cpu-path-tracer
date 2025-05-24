#include <iostream>

int main() {
  constexpr int image_height = 256;
  constexpr int image_width  = 256;

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int i = 0; i < image_height; i++) {
    std::clog << "\rScanlines remaining: " << (image_height - i) << std::flush;
    for (int j = 0; j < image_width; j++) {
      const auto r     = static_cast<double>(j) / (image_width - 1);
      const auto g     = static_cast<double>(i) / (image_height - 1);
      constexpr auto b = 0.0;

      const int ir     = static_cast<int>(255.999 * r);
      const int ig     = static_cast<int>(255.999 * g);
      constexpr int ib = static_cast<int>(255.999 * b);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
  std::clog << "\rDone.                                                     \n";
}