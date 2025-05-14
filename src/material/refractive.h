#pragma once

#include "core/material.h"
#include "utils/macros.h"

namespace RayTracer2D {

class RefractiveMaterial : public Material {
 public:
  explicit RefractiveMaterial(const double r_idx);
  DISALLOW_COPY_AND_MOVE(RefractiveMaterial);

  Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) override;

 private:
  double r_idx_;
};

}  // namespace RayTracer2D
