#pragma once

#include "core/colour.h"
#include "core/light.h"
#include "core/point.h"
#include "core/ray.h"
#include "utils/macros.h"

namespace RayTracer2D {

class LaserLight : public Light {
 public:
  DISALLOW_COPY_AND_MOVE(LaserLight);
  explicit LaserLight(const Point2d &p, const Point2d &d, const Colour &colour);
  Ray GetLightRay() override;

 private:
  Point2d p_, d_;
  Colour colour_;
};

}  // namespace RayTracer2D
