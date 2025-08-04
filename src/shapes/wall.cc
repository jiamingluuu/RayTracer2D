#include "wall.h"
#include "core/material.h"
#include "core/ray.h"

namespace RayTracer2D {

Wall::Wall(const Point2d &begin, const Point2d &end, MaterialPtr material) : p_(begin), d_(end - begin) {
  material_ = std::move(material);
}

std::optional<double> Wall::Intersect(const Ray &ray) const {
  auto cross_product = Cross(ray.d_, d_);

  if (std::abs(cross_product) < PointConstants<double>::kEpsilon) {
    return std::nullopt;
  }

  auto diff = p_ - ray.p_;
  auto t = Cross(diff, d_) / cross_product;
  auto s = Cross(diff, ray.d_) / cross_product;

  if (t >= 0 && s >= 0 && s <= 1) {
    return t;
  }

  return std::nullopt;
}

Point2d Wall::GetNormal(const Ray &ray, const Point2d &p) const {
  auto n = Point2d(-d_.y, d_.x);
  n.Normalize();

  if (Dot(ray.d_, n) > 0) {
    n = -1.0 * n;
  }

  return n;
}

Ray Wall::Interact(const Ray &r, const Point2d &p, const Point2d &n) {
  return material_->Interact(r, p, n);
}

void Wall::Render(Image &image) const {
  Ray(p_, d_, Colour(1, 1, 1)).Render(image, p_ - d_);
}

}  // namespace RayTracer2D
