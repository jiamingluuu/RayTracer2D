
#include "light/point_light.h"

namespace RayTracer2D {

PointLight::PointLight(const Point2d &p, const Colour &colour)
    : p_(p), colour_(colour), gen_(rd_()), distribution_(-1, 1) {}

Ray PointLight::GetLightRay() {
  auto d = Point2d(distribution_(gen_), distribution_(gen_));
  d.Normalize();
  return Ray(p_, d, colour_);
}

}  // namespace RayTracer2D
