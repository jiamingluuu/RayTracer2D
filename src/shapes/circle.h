#pragma once

#include <optional>

#include "core/image.h"
#include "core/material.h"
#include "core/point.h"
#include "core/ray.h"
#include "core/shape.h"

namespace RayTracer2D {

class Circle : public Shape {
 public:
  explicit Circle(const Point2d &c, const double r, MaterialPtr material);

  std::optional<double> Intersect(const Ray &ray) const override;
  Point2d GetNormal(const Ray &ray, const Point2d &p) const override;
  Ray Interact(const Ray &r, const Point2d &p, const Point2d &n) override;
  void Render(Image &image) const override;

 private:
  Point2d c_;
  double r_;
};

}  // namespace RayTracer2D
