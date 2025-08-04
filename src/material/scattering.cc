#include "material/scattering.h"

namespace RayTracer2D {

ScatteringMaterial::ScatteringMaterial() : gen_(rd_()), distribution_(-M_PI / 2, M_PI / 2) {}

Ray ScatteringMaterial::Interact(const Ray &r, const Point2d &p, const Point2d &n) {
  auto theta = distribution_(gen_);
  auto c = cos(theta);
  auto s = sin(theta);
  auto d = Point2d(c * n.x - s * n.y, s * n.x + c * n.y).Normalize();
  return Ray(p, d, r.colour_);
}

}  // namespace RayTracer2D
