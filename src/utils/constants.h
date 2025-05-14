#pragma once

#include "core/point.h"

namespace RayTracer2D {

const auto kTopLeft = Point2d(-2, -2);
const auto kTopRight = Point2d(-2, 2);
const auto kBottomLeft = Point2d(2, -2);
const auto kBottomRight = Point2d(2, 2);

constexpr double kPi = 3.1415926;
constexpr double kEpsilon = 1e-6;

};  // namespace RayTracer2D
