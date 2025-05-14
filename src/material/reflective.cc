#include "material/reflective.h"

namespace RayTracer2D {

Ray ReflectiveMaterial::Interact(const Ray &r, const Point2d &p, const Point2d &n) {
  auto dot = Dot(r.d_, n);
  auto reflected_dir = r.d_ - n * (2.0 * dot);
  return Ray(p, reflected_dir, r.colour_);
}

}  // namespace RayTracer2D
