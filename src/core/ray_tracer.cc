#include "core/ray_tracer.h"
#include <cstdio>
#include <memory>
#include "core/colour.h"
#include "core/point.h"
#include "light/laser_light.h"
#include "material/reflective.h"
#include "material/scattering.h"
#include "utils/constants.h"

namespace RayTracer2D {

static auto parse_args(int argc, char *argv[]) -> Options;

void Main(int argc, char *argv[]) {
  auto option = parse_args(argc, argv);
  auto rt = RayTracer(option);
  for (auto i = 0; i < option.num_rays_; i++) {
    if (option.num_rays_ > 10) {
      if (i % (option.num_rays_ / 10) == 0) {
        fprintf(stderr, "Progress=%f\n", (double)i / (double)(option.num_rays_));
      }
    }
#ifdef Debug
    fprintf(stderr, "Propagating ray %d of %d\n", i, num_rays);
    fprintf(stderr, "Ray is at (%f,%f), direction (%f, %f)\n", ray.p.px, ray.p.py, ray.d.px, ray.d.py);
#endif
    auto ray = rt.light_->GetLightRay();
    rt.PropagateRay(ray, option.depth_);
  }

  rt.image_.AdjustGamma();
  // for (const auto &shape : *rt.scene_) {
  //   shape->Render(*rt.image_);
  // }
  rt.image_.WriteToPPM();
}

RayTracer::RayTracer(const Options &option)
    : image_(option),
      light_(std::make_unique<LaserLight>(Point2d(0, 0), Point2d(1, 0.8).Normalize(), Colour(1, 1, 1))) {
  scene_.AddCircle(Point2d(1.5, -1.5), 0.55, std::make_unique<ScatteringMaterial>());
  scene_.AddCircle(Point2d(0.5, -0.5), 0.25, std::make_unique<ReflectiveMaterial>());
  scene_.AddWall(kTopLeft, kTopRight, std::make_unique<ScatteringMaterial>());
  scene_.AddWall(kTopRight, kBottomRight, std::make_unique<ScatteringMaterial>());
  scene_.AddWall(kBottomRight, kBottomLeft, std::make_unique<ScatteringMaterial>());
  scene_.AddWall(kBottomLeft, kTopLeft, std::make_unique<ScatteringMaterial>());
}

auto parse_args(int argc, char *argv[]) -> Options {
  if (argc < 5) {
    fprintf(stderr, "USAGE: light2D  sx   sy   num_samples   max_depth\n");
    fprintf(stderr, "  sx, sy - image resolution in pixels (in [256 4096])\n");
    fprintf(stderr, "  num_samples - Number of light rays to propagate  (in [1 10,000,000])\n");
    fprintf(stderr, "  max_depth - Maximum recursion depth (in [1 25])\n");
    exit(1);
  }
  auto sx = atoi(argv[1]);
  auto sy = atoi(argv[2]);
  auto num_rays = atoi(argv[3]);
  auto max_depth = atoi(argv[4]);
  if (sx < 256 || sy < 256 || sx > 4096 || sy > 4096 || num_rays < 1 || num_rays > 10000000 || max_depth < 1 ||
      max_depth > 25) {
    fprintf(stderr, "USAGE: light2D  sx   sy   num_samples   max_depth\n");
    fprintf(stderr, "  sx, sy - image resolution in pixels (in [256 4096])\n");
    fprintf(stderr, "  num_samples - Number of light rays to propagate  (in [1 10,000,000])\n");
    fprintf(stderr, "  max_depth - Maximum recursion depth (in [1 25])\n");
    exit(1);
  }
  fprintf(stderr, "Working with:\n");
  fprintf(stderr, "Image size (%d, %d)\n", sx, sy);
  fprintf(stderr, "Number of samples: %d\n", num_rays);
  fprintf(stderr, "Max. recursion depth: %d\n", max_depth);

  return Options(sx, sy, num_rays, max_depth);
}

void RayTracer::PropagateRay(Ray ray, const size_t depth) {
  std::cout << "\n\n## PropagateRay\n";
  for (auto i = 0; i < depth; i++) {
    std::cout << "depth=" << i << '\n';
    auto result = scene_.FindFirstHit(ray);
    if (!result.has_value()) {
      std::cout << "[error]\n";
      exit(1);
      return;
    }
    auto [t_hit, hitted_shape] = result.value();
    auto p = ray(t_hit);
    std::cout << "(ray.p_, p_hit) = " << ray.p_ << ' ' << p << '\n';
    auto n = hitted_shape->GetNormal(ray, p);
    ray.Render(image_, p);
    ray = hitted_shape->Interact(ray, p, n);
    std::cout << "new ray " << ray << '\n';
  }
}

void RayTracer::RenderRay(const Ray &ray, const Point2d &p) {
  double x1, y1, x2, y2, xt, yt;
  int xx, yy;
  double dx, dy;
  double inc;

  if (ray.p_.x < W_LEFT - kEpsilon || ray.p_.x > W_RIGHT + kEpsilon || ray.p_.y < W_TOP - kEpsilon ||
      ray.p_.y > W_BOTTOM + kEpsilon || p.x < W_LEFT - kEpsilon || p.x > W_RIGHT + kEpsilon ||
      ray.p_.y < W_TOP - kEpsilon || p.y > W_BOTTOM + kEpsilon) {
    fprintf(stderr, "RenderRay() - at least one endpoint is outside the image bounds, somewhere there's an error...\n");
    fprintf(stderr, "p1=(%f,%f)\n", ray.p_.x, ray.p_.y);
    fprintf(stderr, "p2=(%f,%f)\n", p.x, p.y);
  }

  x1 = ray.p_.x - W_LEFT;
  y1 = ray.p_.y - W_TOP;
  x2 = p.x - W_LEFT;
  y2 = p.y - W_TOP;

  x1 = x1 / (W_RIGHT - W_LEFT);
  y1 = y1 / (W_BOTTOM - W_TOP);
  x2 = x2 / (W_RIGHT - W_LEFT);
  y2 = y2 / (W_BOTTOM - W_TOP);

  x1 = x1 * (image_.sx_ - 1);
  y1 = y1 * (image_.sy_ - 1);
  x2 = x2 * (image_.sx_ - 1);
  y2 = y2 * (image_.sy_ - 1);

  dx = x2 - x1;
  dy = y2 - y1;

  if (abs(dx) >= abs(dy)) {
    if (x2 < x1) {
      xt = x1;
      yt = y1;
      x1 = x2;
      y1 = y2;
      x2 = xt;
      y2 = yt;
    }

    yt = y1;
    inc = (y2 - y1) / abs(x2 - x1);
    for (double xt = x1; xt <= x2; xt += 1) {
      xx = (int)round(xt);
      yy = (int)round(yt);
      if (xx >= 0 && xx < image_.sx_ && yy >= 0 && yy < image_.sy_) {
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 0] += ray.colour_.R_;
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 1] += ray.colour_.G_;
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 2] += ray.colour_.B_;
      }
      yt += inc;
    }

  } else {
    if (y2 < y1) {
      xt = x1;
      yt = y1;
      x1 = x2;
      y1 = y2;
      x2 = xt;
      y2 = yt;
    }

    xt = x1;
    inc = (x2 - x1) / abs(y2 - y1);
    for (double yt = y1; yt <= y2; yt += 1) {
      xx = (int)round(xt);
      yy = (int)round(yt);
      if (xx >= 0 && xx < image_.sx_ && yy >= 0 && yy < image_.sy_) {
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 0] += ray.colour_.R_;
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 1] += ray.colour_.G_;
        image_.data_[(xx + (yy * image_.sx_)) * 3 + 2] += ray.colour_.B_;
      }
      xt += inc;
    }
  }
}

}  // namespace RayTracer2D
