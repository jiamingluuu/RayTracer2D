#pragma once

#include <memory>
#include "core/point.h"
#include "core/ray.h"

namespace RayTracer2D {

class Material {
 public:
  virtual ~Material() = default;

  /** @return the ray after incident ray interacts with the hitted  object. */
  virtual Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) = 0;
};

using MaterialPtr = std::unique_ptr<Material>;

}  // namespace RayTracer2D
