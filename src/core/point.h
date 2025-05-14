#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace RayTracer2D {

template <typename T>
struct PointConstants {
  static constexpr T kEpsilon = std::is_same<T, float>::value ? 1e-5f : 1e-9;
  static constexpr T kMinLengthSquared = std::is_same<T, float>::value ? 1e-12f : 1e-24;
};

template <typename T>
class Point {
 public:
  explicit Point() = default;
  explicit Point(T x_, T y_) : x(x_), y(y_) {
    assert(!HasNaNs());
  }

  Point<T> &operator=(const Point<T> other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  T operator[](const size_t i) {
    assert(0 <= i && i <= 2);
    if (i == 0) {
      return x;
    } else {
      return y;
    }
  }

  T Length() const {
    return std::sqrt(LengthSquared());
  }

  T LengthSquared() const {
    return x * x + y * y;
  }

  Point &Normalize() {
    assert(!HasNaNs());
    const T length_sq = LengthSquared();

    // Avoid division by very small numbers
    if (length_sq > PointConstants<T>::kMinLengthSquared) {
      // Compute inverse length once and multiply (faster than division)
      const T length_inv = std::sqrt(length_sq);
      x /= length_inv;
      y /= length_inv;
    } else {
      // For near-zero vectors, default to x-axis
      x = 1;
      y = 0;
    }
    return *this;
  }

  Point<T> Normalized() const {
    Point<T> result = *this;
    result.Normalize();
    return result;
  }

  Point<T> operator+(const Point<T> &p) const {
    assert(!HasNaNs());
    assert(!p.HasNaNs());

    return Point<T>(x + p.x, y + p.y);
  }

  Point<T> operator-(const Point<T> &p) const {
    assert(!HasNaNs());
    assert(!p.HasNaNs());

    return Point<T>(x - p.x, y - p.y);
  }

  Point<T> operator*(const T r) const {
    assert(!HasNaNs());

    return Point<T>(x * r, y * r);
  }

  Point<T> operator/(const T r) const {
    assert(!HasNaNs());
    assert(r != 0);

    // Trust the compiler optimiztion
    return Point<T>(x / r, y / r);
  }

  Point<T> &operator+=(const Point<T> &p) {
    assert(!HasNaNs());
    assert(!p.HasNaNs());

    x += p.x;
    y += p.y;
    return *this;
  }

  Point<T> &operator-=(const Point<T> &p) {
    assert(!HasNaNs());
    assert(!p.HasNaNs());

    x -= p.x;
    y -= p.y;
    return *this;
  }

  Point<T> &operator*=(const T r) {
    assert(!HasNaNs());

    x *= r;
    y *= r;
    return *this;
  }

  Point<T> &operator/=(const T r) {
    assert(!HasNaNs());
    assert(r != 0);

    // Trust compiler optimization for simple division operations
    x /= r;
    y /= r;
    return *this;
  }

  bool operator==(const Point<T> &p) const {
    return x == p.x && y == p.y;
  }

  bool operator!=(const Point<T> &p) const {
    return x != p.x || y != p.y;
  }

  bool ApproxEquals(const Point<T> &p, T eps = PointConstants<T>::kEpsilon) const {
    return std::abs(x - p.x) <= eps * std::max(std::abs(x), std::abs(p.x) + static_cast<T>(1)) &&
           std::abs(y - p.y) <= eps * std::max(std::abs(y), std::abs(p.y) + static_cast<T>(1));
  }

  bool HasNaNs() const {
    return std::isnan(x) || std::isnan(y);
  }

  friend std::ostream &operator<<(std::ostream &os, const Point<T> &r) {
    os << "(" << r.x << ", " << r.y << ")";
    return os;
  }

 public:
  T x;
  T y;
};

// Optimized dot product
template <typename T>
T Dot(const Point<T> &u, const Point<T> &v) {
  assert(!u.HasNaNs() && !v.HasNaNs());

  return u.x * v.x + u.y * v.y;
}

template <typename T>
T Cross(const Point<T> &u, const Point<T> &v) {
  assert(!u.HasNaNs() && !v.HasNaNs());

  return u.x * v.y - u.y * v.x;
}

template <typename T>
Point<T> operator*(T s, const Point<T> &p) {
  return p * s;
}

using Point2d = Point<double>;
using Point2f = Point<float>;
using Point2i = Point<int>;

}  // namespace RayTracer2D
