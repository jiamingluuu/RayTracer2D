#pragma once

#include <memory>
#include "core/image.h"
#include "core/options.h"
#include "core/point.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/light.h"

namespace RayTracer2D {

void Main(int argc, char *argv[]);

class RayTracer {
 public:
  RayTracer(const Options &option);
  void PropagateRay(Ray ray, const size_t depth);
  void RenderRay(const Ray &ray, const Point2d &p);

 public:
  Scene scene_;
  Image image_;
  std::unique_ptr<Light> light_;
};

}  // namespace RayTracer2D
