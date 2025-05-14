#include "core/colour.h"

namespace RayTracer2D {

Colour::Colour(const double R, const double G, const double B) : R_(R), G_(G), B_(B) {}

bool Colour::ValidateColour() const {
  return (0 <= R_ && R_ <= 1) && (0 <= G_ && G_ <= 1) && (0 <= B_ && B_ <= 1);
}

}  // namespace RayTracer2D
