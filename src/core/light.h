#pragma once

#include "core/ray.h"

namespace RayTracer2D {

class Light {
 public:
  Light() = default;
  virtual ~Light() = default;

  virtual Ray GetLightRay() = 0;
};

};  // namespace RayTracer2D
