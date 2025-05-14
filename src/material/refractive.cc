#include "material/refractive.h"

namespace RayTracer2D {

RefractiveMaterial::RefractiveMaterial(double r_idx) : r_idx_(r_idx) {}

Ray RefractiveMaterial::Interact(const Ray &r, const Point2d &p, const Point2d &n) {
  auto is_entering = !r.is_inside_object_;
  auto refracted_dir = r.d_ + n * (is_entering ? -0.1 : 0.1);
  refracted_dir.Normalize();

  auto refracted_ray = Ray(p, refracted_dir, r.colour_);
  refracted_ray.is_inside_object_ = !r.is_inside_object_;
  return refracted_ray;
}

}  // namespace RayTracer2D
