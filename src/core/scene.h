#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "core/material.h"
#include "core/point.h"
#include "core/shape.h"
#include "utils/macros.h"

namespace RayTracer2D {

class Scene {
 public:
  Scene() = default;

  // You can delete this line on need. We delete it here just to prevent
  // the misuse of this data class.
  DISALLOW_COPY_AND_MOVE(Scene);

  void AddWall(const Point2d &begin, const Point2d &end, MaterialPtr material);
  void AddCircle(const Point2d &center, const double r, MaterialPtr material);
  auto FindFirstHit(const Ray &ray) const -> std::optional<std::pair<double, Shape *>>;

  auto begin() {
    return shapes_.begin();
  }
  auto end() {
    return shapes_.end();
  }

  auto begin() const {
    return shapes_.begin();
  }
  auto end() const {
    return shapes_.end();
  }

  auto cbegin() const {
    return shapes_.cbegin();
  }
  auto cend() const {
    return shapes_.cend();
  }

 private:
  std::vector<std::unique_ptr<Shape>> shapes_;
};

};  // namespace RayTracer2D
