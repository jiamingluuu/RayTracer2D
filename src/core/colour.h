#pragma once

namespace RayTracer2D {

struct Colour {
  explicit Colour(const double R, const double G, const double B);

  auto operator*(const Colour &other) const -> Colour {
    return Colour(R_ * other.R_, G_ * other.G_, B_ * other.B_);
  }

  auto operator*(const double r) const -> Colour {
    return Colour(R_ * r, G_ * r, B_ * r);
  }

  auto operator*=(const Colour &other) {
    R_ *= other.R_;
    G_ *= other.G_;
    B_ *= other.B_;
  }

  auto operator*=(const double r) {
    R_ *= r;
    G_ *= r;
    B_ *= r;
  }


  bool ValidateColour() const;

  double R_;
  double G_;
  double B_;
};

}  // namespace RayTracer2D
