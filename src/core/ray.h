#pragma once

#include <iostream>
#include "core/colour.h"
#include "core/image.h"
#include "core/point.h"

namespace RayTracer2D {

class Ray {
 public:
  explicit Ray(const Point2d &p, const Point2d &d, const Colour &colour);

  auto operator()(const double t) const -> Point2d;

  friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "{ p=" << r.p_ << ", d=" << r.d_ << " }";
    return os;
  }

  void Render(const Image &image, const Point2d &p) const;

 public:
  // Position and direction vector of the ray.
  Point2d p_;
  Point2d d_;

  // Colour of this light ray
  Colour colour_;

  bool is_inside_object_{false};

  // Flag to indicate whether this ray is monochromatic (true if set to 1)
  bool is_monochromatic_{false};

  // For monochromatic rays, HUE value (used to obtain colour, and as a
  // convenient substitute for wavelength) values in [0 1] go from deep red to
  // purple.
  double H;
};

}  // namespace RayTracer2D
