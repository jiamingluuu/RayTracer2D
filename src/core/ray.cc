#include "core/ray.h"
#include "core/colour.h"
#include "utils/constants.h"

namespace RayTracer2D {

Ray::Ray(const Point2d &p, const Point2d &d, const Colour &colour) : p_(p), d_(d), colour_(colour) {}

Point2d Ray::operator()(const double t) const {
  return p_ + d_ * t;
}

void Ray::Render(const Image &image, const Point2d &p) const {
  double x1, y1, x2, y2, xt, yt;
  int xx, yy;
  double dx, dy;
  double inc;

  if (p_.x < W_LEFT - kEpsilon || p_.x > W_RIGHT + kEpsilon || p_.y < W_TOP - kEpsilon || p_.y > W_BOTTOM + kEpsilon ||
      p.x < W_LEFT - kEpsilon || p.x > W_RIGHT + kEpsilon || p.y < W_TOP - kEpsilon || p.y > W_BOTTOM + kEpsilon) {
    std::cerr << "Ray::Render() - at least one endpoint is outside the image bounds, somewhere there's an error...\n";
    std::cerr << "p1=(" << p_.x << "," << p_.y << ")\n";
    std::cerr << "p2=(" << p.x << "," << p.y << ")\n";
  }

  x1 = p_.x - W_LEFT;
  y1 = p_.y - W_TOP;
  x2 = p.x - W_LEFT;
  y2 = p.y - W_TOP;

  x1 = x1 / (W_RIGHT - W_LEFT);
  y1 = y1 / (W_BOTTOM - W_TOP);
  x2 = x2 / (W_RIGHT - W_LEFT);
  y2 = y2 / (W_BOTTOM - W_TOP);

  x1 = x1 * (image.sx_ - 1);
  y1 = y1 * (image.sy_ - 1);
  x2 = x2 * (image.sx_ - 1);
  y2 = y2 * (image.sy_ - 1);

  dx = x2 - x1;
  dy = y2 - y1;

  if (std::abs(dx) >= std::abs(dy)) {
    if (x2 < x1) {
      std::swap(x1, x2);
      std::swap(y1, y2);
    }

    yt = y1;
    inc = (y2 - y1) / std::abs(x2 - x1);
    for (double xt = x1; xt <= x2; xt += 1) {
      xx = static_cast<int>(std::round(xt));
      yy = static_cast<int>(std::round(yt));
      if (xx >= 0 && xx < image.sx_ && yy >= 0 && yy < image.sy_) {
        image.data_[(xx + (yy * image.sx_)) * 3 + 0] += colour_.R_;
        image.data_[(xx + (yy * image.sx_)) * 3 + 1] += colour_.G_;
        image.data_[(xx + (yy * image.sx_)) * 3 + 2] += colour_.B_;
      }
      yt += inc;
    }
  } else {
    if (y2 < y1) {
      std::swap(x1, x2);
      std::swap(y1, y2);
    }

    xt = x1;
    inc = (x2 - x1) / std::abs(y2 - y1);
    for (double yt = y1; yt <= y2; yt += 1) {
      xx = static_cast<int>(std::round(xt));
      yy = static_cast<int>(std::round(yt));
      if (xx >= 0 && xx < image.sx_ && yy >= 0 && yy < image.sy_) {
        image.data_[(xx + (yy * image.sx_)) * 3 + 0] += colour_.R_;
        image.data_[(xx + (yy * image.sx_)) * 3 + 1] += colour_.G_;
        image.data_[(xx + (yy * image.sx_)) * 3 + 2] += colour_.B_;
      }
      xt += inc;
    }
  }
}

}  // namespace RayTracer2D
