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
#ifndef SPAGHETTI_ELEMENTS_OPENCV_CAP_H
#define SPAGHETTI_ELEMENTS_OPENCV_CAP_H

#include <spaghetti/element.h>
#include <opencv2/videoio/videoio.hpp>

class CapAsync {
 public:
  CapAsync();
  ~CapAsync();

  void release();
  bool open(std::string const a_name);
  bool open(int const a_camID);
  bool isOpened();
  bool hasNewFrame();
  cv::Mat grabFrame();

 private:
  std::thread m_captureThread{};
  cv::VideoCapture m_cap{};
  bool m_hasNewFrame{};
  cv::Mat m_frame{};
  bool m_isOpened{};
  bool m_killThread{};

  void runCapture();
  static void capture(CapAsync *a_context);
};

namespace spaghetti::elements {
class VideoCapture final : public Element {
 public:
  static constexpr char const *const TYPE{ "OpenCV/VideoIO/video_capture" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  VideoCapture();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

 private:
  CapAsync m_cap{};

  std::string m_sourceStr{};
  int m_sourceID{};
  ValueType m_sourceType{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_OPENCV_CAP_H
