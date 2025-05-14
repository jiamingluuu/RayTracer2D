#pragma once

#include <random>

#include "core/colour.h"
#include "core/light.h"
#include "core/point.h"
#include "utils/macros.h"

namespace RayTracer2D {

class PointLight : public Light {
 public:
  DISALLOW_COPY_AND_MOVE(PointLight);
  explicit PointLight(const Point2d &p, const Colour &colour);
  Ray GetLightRay() override;

 private:
  Point2d p_;
  Colour colour_;

  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_real_distribution<double> distribution_;
};

}  // namespace RayTracer2D
