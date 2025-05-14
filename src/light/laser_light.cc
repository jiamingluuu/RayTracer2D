#include "light/laser_light.h"
#include <cassert>

#include "core/point.h"
#include "utils/constants.h"

namespace RayTracer2D {

LaserLight::LaserLight(const Point2d &p, const Point2d &d, const Colour &colour) : p_(p), d_(d), colour_(colour) {
  assert(abs(d_.Length() - 1) < kEpsilon);  // Laser light source need to have unit-lengthed directional vector.
}

Ray LaserLight::GetLightRay() {
  return Ray(p_, d_, colour_);
}

}  // namespace RayTracer2D
