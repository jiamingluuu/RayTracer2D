#pragma once

#include <optional>

#include "core/image.h"
#include "core/material.h"
#include "core/point.h"
#include "core/ray.h"
#include "core/shape.h"

namespace RayTracer2D {

class Wall : public Shape {
 public:
  explicit Wall(const Point2d &begin, const Point2d &end, MaterialPtr material);

  std::optional<double> Intersect(const Ray &ray) const override;
  Point2d GetNormal(const Ray &ray, const Point2d &p) const override;
  Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) override;
  void Render(const Image &image) const override;

 private:
  Point2d p_;
  Point2d d_;
};

}  // namespace RayTracer2D
