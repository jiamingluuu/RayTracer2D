#pragma once

#include <cstddef>
namespace RayTracer2D {

struct Options {
  explicit Options(size_t sx, size_t sy, size_t num_rays, size_t depth)
      : sx_(sx), sy_(sy), num_rays_(num_rays), depth_(depth) {}

  size_t sx_, sy_;
  size_t num_rays_;
  size_t depth_;
};

}  // namespace RayTracer2D
