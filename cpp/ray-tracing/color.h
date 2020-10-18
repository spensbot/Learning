#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "image.h"

#include <iostream>

int convert(double normalized) {
  return static_cast<int>(256 * clamp(normalized, 0.0, 0.999));
}

Pixel get_color(color pixel_color, int samples_per_pixel)
{
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples and gamma-correct for gamma=2.0.
  auto scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b); 

  return {convert(r), convert(g), convert(b)};
}

#endif