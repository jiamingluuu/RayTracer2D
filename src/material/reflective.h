#pragma once

#include "core/material.h"
#include "utils/macros.h"

namespace RayTracer2D {

class ReflectiveMaterial : public Material {
 public:
  ReflectiveMaterial() = default;
  DISALLOW_COPY_AND_MOVE(ReflectiveMaterial);

  Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) override;
};

}  // namespace RayTracer2D
