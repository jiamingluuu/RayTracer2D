#pragma once

#include <memory>
#include <optional>

#include "core/image.h"
#include "core/material.h"
#include "core/ray.h"
#include "point.h"

namespace RayTracer2D {

class Shape {
 public:
  Shape() = default;
  virtual ~Shape() = default;

  // Return the time of ray propagates on which it collides with the shape.
  virtual auto Intersect(const Ray &ray) const -> std::optional<double> = 0;

  // Obtain the normal that is pointing towards `ray`, that is, the angle
  // between the directional vector of `ray` and the returned normal forms
  // an obtuse angle.
  virtual auto GetNormal(const Ray &ray, const Point2d &p_h) const -> Point2d = 0;

  // Return the new spawned ray after hitting the object.
  virtual auto Interact(const Ray &r, const Point2d &p, const Point2d &n) -> Ray = 0;

  // Render the object on canvas (for debug purpose).
  virtual void Render(Image &image) const = 0;

 protected:
  MaterialPtr material_;
};

using ShapePtr = std::unique_ptr<Shape>;

}  // namespace RayTracer2D
