#include "shapes/circle.h"
#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include "core/material.h"
#include "core/point.h"
#include "material/reflective.h"
#include "material/refractive.h"
#include "material/scattering.h"

namespace RayTracer2D {

static constexpr double kEpsilon = 1e-6;

class CircleTest : public ::testing::Test {
 protected:
  CircleTest()
      : reflective_circle(Point2d(0, 0), 1.0, std::make_unique<ReflectiveMaterial>()),
        scattering_circle(Point2d(3, 4), 2.0, std::make_unique<ScatteringMaterial>()),
        refractive_circle(Point2d(-2, -2), 1.5, std::make_unique<RefractiveMaterial>(0.9)) {}

  void SetUp() override {}
  void TearDown() override {}

  void ExpectPointsNearEqual(const Point2d &p1, const Point2d &p2, double epsilon = kEpsilon) {
    EXPECT_NEAR(p1.x, p2.x, epsilon);
    EXPECT_NEAR(p1.y, p2.y, epsilon);
  }

  void VerifyNormal(const Point2d &normal) { EXPECT_NEAR(normal.Length(), 1.0, kEpsilon); }

  Circle reflective_circle;
  Circle scattering_circle;
  Circle refractive_circle;
};

TEST_F(CircleTest, Constructor) {
  auto circle = Circle(Point2d(5, 10), 3.0, std::make_unique<RefractiveMaterial>(0.9));
  auto ray = Ray(Point2d(0, 10), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = circle.Intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection.value(), 2.0, kEpsilon);
}

TEST_F(CircleTest, IntersectHitFromOutside) {
  auto ray = Ray(Point2d(3, 0), Point2d(-1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = reflective_circle.Intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection.value(), 2.0, 1e-10);
}

// Test Intersect method for rays missing the circle
TEST_F(CircleTest, IntersectMiss) {
  // Ray that passes above the circle
  auto ray = Ray(Point2d(3, 2), Point2d(-1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = reflective_circle.Intersect(ray);

  ASSERT_FALSE(intersection.has_value());
}

// Test Intersect method for rays starting inside the circle
TEST_F(CircleTest, IntersectFromInside) {
  // Ray starting inside and pointing outward
  auto ray = Ray(Point2d(0.5, 0), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = reflective_circle.Intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  // Should hit at the boundary where x = 1
  EXPECT_NEAR(intersection.value(), 0.5, kEpsilon);
}

// Test Intersect method for tangent rays
TEST_F(CircleTest, IntersectTangent) {
  // Ray that is tangent to the circle at (0, 1)
  auto ray = Ray(Point2d(-3, 1), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = reflective_circle.Intersect(ray);

  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection.value(), 3.0, kEpsilon);
}

// Test Intersect method for rays starting on the boundary
TEST_F(CircleTest, IntersectFromBoundary) {
  // Ray starting on the boundary and pointing outward
  auto ray = Ray(Point2d(1, 0), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = reflective_circle.Intersect(ray);

  // Ideally would return 0, but may not due to numerical issues
  // We'll accept either no intersection or t ≈ 0
  if (intersection.has_value()) {
    EXPECT_NEAR(intersection.value(), 0.0, kEpsilon);
  }
}

TEST_F(CircleTest, GetNormalFromOutside) {
  auto ray = Ray(Point2d(3, 0), Point2d(-1, 0), Colour(1.0, 1.0, 1.0));
  auto hit_point = Point2d(1, 0);
  auto normal = reflective_circle.GetNormal(ray, hit_point);

  VerifyNormal(normal);
  ExpectPointsNearEqual(normal, Point2d(1, 0));
}

TEST_F(CircleTest, GetNormalFromInside) {
  auto ray = Ray(Point2d(0.5, 0), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  ray.is_inside_object_ = true;
  auto hit_point = Point2d(1, 0);
  auto normal = reflective_circle.GetNormal(ray, hit_point);

  VerifyNormal(normal);
  ExpectPointsNearEqual(normal, Point2d(-1, 0));
}

TEST_F(CircleTest, InteractReflective) {
  auto ray = Ray(Point2d(3, 0), Point2d(-1, 0), Colour(0.5, 0.6, 0.7));
  auto hit_point = Point2d(1, 0);
  auto normal = Point2d(1, 0);

  auto reflected = reflective_circle.Interact(ray, hit_point, normal);

  ExpectPointsNearEqual(reflected.p_, hit_point);
  ExpectPointsNearEqual(reflected.d_, Point2d(1, 0));
  EXPECT_EQ(reflected.is_inside_object_, ray.is_inside_object_);
}

TEST_F(CircleTest, InteractScattering) {
  auto ray = Ray(Point2d(5, 4), Point2d(-1, 0), Colour(0.5, 0.6, 0.7));
  auto hit_point = Point2d(5, 4);
  auto normal = Point2d(1, 0);

  auto scattered = scattering_circle.Interact(ray, hit_point, normal);

  ExpectPointsNearEqual(scattered.p_, hit_point);
  ExpectPointsNearEqual(scattered.d_, normal);
}

TEST_F(CircleTest, InteractRefractiveEntering) {
  auto ray = Ray(Point2d(0, 0), Point2d(-1, 0), Colour(0.5, 0.6, 0.7));
  auto hit_point = Point2d(-0.5, 0);
  auto normal = Point2d(1, 0);

  auto refracted = refractive_circle.Interact(ray, hit_point, normal);

  ExpectPointsNearEqual(refracted.p_, hit_point);
  EXPECT_TRUE(refracted.is_inside_object_);

  EXPECT_LT(Dot(ray.d_, refracted.d_), 0);
  EXPECT_NEAR(refracted.d_.Length(), 1.0, kEpsilon);
}

TEST_F(CircleTest, InteractRefractiveExiting) {
  auto ray = Ray(Point2d(-1, -2), Point2d(0, 1), Colour(0.5, 0.6, 0.7));
  ray.is_inside_object_ = true;
  auto hit_point = Point2d(-1, -0.5);
  auto normal = Point2d(0, -1);
  auto refracted = Ray(refractive_circle.Interact(ray, hit_point, normal));

  ExpectPointsNearEqual(refracted.p_, hit_point);
  EXPECT_FALSE(refracted.is_inside_object_);

  EXPECT_NEAR(refracted.d_.Length(), 1.0, kEpsilon);
}

TEST_F(CircleTest, EdgeCases) {
  auto tiny_circle = Circle(Point2d(0, 0), kEpsilon, std::make_unique<ReflectiveMaterial>());
  auto ray = Ray(Point2d(1, 0), Point2d(-1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = tiny_circle.Intersect(ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_GT(intersection.value(), 0.999);

  auto parallel_ray = Ray(Point2d(0, 2), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  intersection = reflective_circle.Intersect(parallel_ray);
  ASSERT_FALSE(intersection.has_value());

  auto non_norm_ray = Ray(Point2d(3, 0), Point2d(-2, 0), Colour(1.0, 1.0, 1.0));
  intersection = reflective_circle.Intersect(non_norm_ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection.value(), 1.0, kEpsilon);
}

TEST_F(CircleTest, NumericalStability) {
  auto far_circle = Circle(Point2d(1e6, 1e6), 1.0, std::make_unique<ReflectiveMaterial>());
  auto far_ray = Ray(Point2d(1e6 - 10, 1e6), Point2d(1, 0), Colour(1.0, 1.0, 1.0));
  auto intersection = far_circle.Intersect(far_ray);
  ASSERT_TRUE(intersection.has_value());
  EXPECT_NEAR(intersection.value(), 9.0, kEpsilon);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace RayTracer2D
