#pragma once

#include <cstddef>
#include <cstdlib>
#include "core/colour.h"
#include "core/options.h"
#include "utils/macros.h"

namespace RayTracer2D {

class Image {
 public:
  Image(const Options &option);
  Image(Image &&image);
  ~Image();

  // We disallow copy constructor to prevent double free of the 
  // 'data_' pointer on instance distruction.
  DISALLOW_COPY(Image);

  void AdjustGamma();
  void WriteToPPM();
  void SetPixel(double x, double y, const Colour &colour);

 public:
  double *data_;
  size_t sx_, sy_;
};

}  // namespace RayTracer2D
