#pragma once

#include <random>
#include "core/material.h"
#include "utils/macros.h"

namespace RayTracer2D {

class ScatteringMaterial : public Material {
 public:
  ScatteringMaterial();
  DISALLOW_COPY_AND_MOVE(ScatteringMaterial);

  Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) override;

 private:
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_real_distribution<double> distribution_;
};

}  // namespace RayTracer2D
