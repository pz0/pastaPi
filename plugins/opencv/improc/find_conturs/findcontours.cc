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

#include "findcontours.h"
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {
FindConturs::FindConturs()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addOutput(ValueType::eShapeVector, "Contours", IOSocket::eCanHoldShapeVector | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "N", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
}

void FindConturs::calculate()
{
  auto sourceImage = m_inputs[0].getValue<cv::Mat>();

  if (!sourceImage.empty()) {
    ShapeVector contours{};
    std::vector<cv::Vec4i> hierarchy{};

    if (sourceImage.type() != CV_8UC1) {
      cv::cvtColor(sourceImage, sourceImage, cv::COLOR_BGR2GRAY);
    }

    cv::findContours(sourceImage, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    m_outputs[0].setValue(contours);
    m_outputs[1].setValue(static_cast<int>(contours.size()));
  }
}

} // namespace spaghetti::elements
