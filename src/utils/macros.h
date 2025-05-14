#pragma once

#include <cassert>
#include <exception>
#include <stdexcept>
#include "core/point.h"

namespace RayTracer2D {

#define UNIMPLEMENTED(message) throw std::logic_error(message)
#define UNREACHABLE(message) throw std::logic_error(message)

// Macros to disable copying and moving
#define DISALLOW_COPY(cname)                                    \
  cname(const cname &) = delete;                   /* NOLINT */ \
  auto operator=(const cname &)->cname & = delete; /* NOLINT */

#define DISALLOW_MOVE(cname)                               \
  cname(cname &&) = delete;                   /* NOLINT */ \
  auto operator=(cname &&)->cname & = delete; /* NOLINT */

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname);               \
  DISALLOW_MOVE(cname);

#define W_TOP -2.00
#define W_BOTTOM 2.00
#define W_LEFT -2.00
#define W_RIGHT 2.00

}  // namespace RayTracer2D
