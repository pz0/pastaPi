// MIT License
//
// Copyright (c) 2020 Paweł Adamski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef SPAGHETTI_ELEMENTS_OPENCV_CROP_IMAGE_H
#define SPAGHETTI_ELEMENTS_OPENCV_CROP_IMAGE_H

#include <spaghetti/element.h>

namespace spaghetti::elements::opencv {
class CropImage final : public Element {
 public:
  static constexpr char const *const TYPE{ "opencv/crop_image" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  CropImage();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

 private:
  std::chrono::high_resolution_clock::time_point m_lastFrameTimeStamp{};
  int m_x{};
  int m_y{};
  int m_width{};
  int m_height{};
};

} // namespace spaghetti::elements::opencv

#endif // SPAGHETTI_ELEMENTS_OPENCV_CROP_IMAGE_H
