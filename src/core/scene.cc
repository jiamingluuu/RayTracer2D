#include "core/scene.h"
#include <limits>
#include <memory>
#include <optional>
#include <utility>
#include "shapes/circle.h"
#include "shapes/wall.h"

namespace RayTracer2D {

void Scene::AddWall(const Point2d &begin, const Point2d &end, MaterialPtr material) {
  shapes_.push_back(std::make_unique<Wall>(begin, end, std::move(material)));
}

void Scene::AddCircle(const Point2d &center, const double r, MaterialPtr material) {
  shapes_.push_back(std::make_unique<Circle>(center, r, std::move(material)));
}

auto Scene::FindFirstHit(const Ray &ray) const -> std::optional<std::pair<double, Shape *>> {
  auto t_min = std::numeric_limits<double>().infinity();
  Shape *hitted_shape = nullptr;
  for (const auto &shape : shapes_) {
    auto result = shape->Intersect(ray);
    if (result.has_value()) {
      if (result.value() < t_min) {
        t_min = result.value();
        hitted_shape = shape.get();
      }
    }
  }

  if (hitted_shape == nullptr) {
    return std::nullopt;
  } else {
    return std::make_pair(t_min, hitted_shape);
  }
}

};  // namespace RayTracer2D
