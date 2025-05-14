#include "circle.h"
#include <cmath>
#include "core/material.h"

namespace RayTracer2D {

Circle::Circle(const Point2d &c, const double r, MaterialPtr material) : c_(c), r_(r) {
  material_ = std::move(material);
}

std::optional<double> Circle::Intersect(const Ray &ray) const {
  auto oc = ray.p_ - c_;
  auto a = Dot(ray.d_, ray.d_);
  auto b = 2.0 * Dot(ray.d_, oc);
  auto c = Dot(oc, oc) - r_ * r_;

  auto discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return std::nullopt;
  }

  auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  if (t1 > 0) {
    return t1;
  } else if (t2 > 0) {
    return t2;
  } else {
    // Both intersections are behind the ray's origin
    return std::nullopt;
  }
}

Point2d Circle::GetNormal(const Ray &ray, const Point2d &p) const {
  // The normal is from the center of the circle to the hit point
  auto normal = p - c_;
  normal.Normalize();

  // Ensure the normal points towards the ray (forms an obtuse angle with the
  // ray direction) If the dot product of the ray direction and the normal is
  // positive, they form an acute angle, so we need to flip the normal
  if (Dot(ray.d_, normal) > 0) {
    normal = normal * -1.0;
  }

  return normal;
}

Ray Circle::Interact(const Ray &r, const Point2d &p, const Point2d &n) {
  return material_->Interact(r, p, n);
}

void Circle::Render(const Image &image) const {
  double x, y;
  int xx, yy;
  for (double ang = 0; ang < 2 * M_PI; ang += .001) {
    x = c_.x + (cos(ang) * r_);
    y = c_.y + (sin(ang) * r_);
    x -= W_LEFT;
    y -= W_TOP;
    x = x / (W_RIGHT - W_LEFT);
    y = y / (W_BOTTOM - W_TOP);
    x = x * (image.sx_ - 1);
    y = y * (image.sy_ - 1);
    xx = (int)round(x);
    yy = (int)round(y);
    if (0 <= xx && xx < image.sx_ && 0 <= yy && yy < image.sy_) {
      image.data_[(xx + (yy * image.sx_)) * 3 + 0] = 0;
      image.data_[(xx + (yy * image.sx_)) * 3 + 1] = 255;
      image.data_[(xx + (yy * image.sx_)) * 3 + 2] = 0;
    }
  }
}

}  // namespace RayTracer2D
