#include "core/image.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include "core/colour.h"

namespace RayTracer2D {

Image::Image(const Options &option) : sx_(option.sx_), sy_(option.sy_) {
  data_ = new double[sx_ * sy_ * 3]();
}

Image::Image(Image &&other) {
  data_ = other.data_;
  other.data_ = nullptr;
}

Image::~Image() {
  delete[] data_;
}

void Image::AdjustGamma() {
  for (auto i = 0; i < 3 * sx_ * sy_; i++) {
    data_[i] = log(data_[i] + 1.5);
  }
}

void Image::WriteToPPM() {
  double image_max = -1;
  double image_min = 1e6;
  for (auto i = 0; i < 3 * sx_ * sy_; i++) {
    image_max = std::fmax(image_max, data_[i]);
    image_min = std::fmin(image_min, data_[i]);
  }
  auto image_range = image_max - image_min;
  fprintf(stderr, "%f %f %f\n", image_max, image_min, image_range);
  auto ppm_image = new unsigned char[3 * sx_ * sy_]();
  for (auto i = 0; i < 3 * sx_ * sy_; i++) {
    ppm_image[i] += static_cast<unsigned char>(255.0 * ((data_[i] - image_min) / image_range));
  }

  for (const auto &map: maps_) {
    for (auto i = 0; i < 3 * sx_ * sy_; i++) {
      ppm_image[i] += map[i];
    }
  }

  std::ofstream ofs("output.ppm", std::ios::binary);
  if (!ofs) {
    std::cerr << "can not create output image file" << std::endl;
    return;
  }
  ofs << "P6\n";
  ofs << "# Output from Light2D.c\n";
  ofs << sx_ << " " << sy_ << "\n";
  ofs << "255\n";
  ofs.write(reinterpret_cast<const char *>(ppm_image), sx_ * sy_ * 3 * sizeof(unsigned char));
}

void Image::SetPixel(double x, double y, const Colour &colour) {
  assert(colour.ValidateColour());
  x -= W_LEFT;
  y -= W_TOP;
  x = x / (W_RIGHT - W_LEFT);
  y = y / (W_BOTTOM - W_TOP);
  x = x * (sx_ - 1);
  y = y * (sy_ - 1);

  auto xx = static_cast<size_t>(round(x));
  auto yy = static_cast<size_t>(round(y));
  for (auto i = xx - 1; i <= xx + 1; i++) {
    for (auto j = yy - 1; j <= yy + 1; j++) {
      auto W = exp(-(((x - i) * (x - i)) + ((y - j) * (y - j))) * .5);
      if (i >= 0 && j >= 0 && i < sx_ && j < sy_) {
        data_[(i + (j * sx_)) * 3 + 0] += W * colour.R_;
        data_[(i + (j * sx_)) * 3 + 1] += W * colour.G_;
        data_[(i + (j * sx_)) * 3 + 2] += W * colour.B_;
      }
    }
  }
}

}  // namespace RayTracer2D
