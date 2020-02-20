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

#include <spaghetti/elements/opencv/mog2.h>
#include <spaghetti/utils.h>

namespace spaghetti::elements::opencv {
Mog2::Mog2()
  : Element{}
{
  setMinInputs(4);
  setMaxInputs(4);
  setMinOutputs(1);
  setMaxOutputs(2);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "History", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "Threshold", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "DetectShadows", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrix, "ForegroundMask", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanChangeName);
  setDefaultNewOutputFlags(IOSocket::eCanChangeName | IOSocket::eCanHoldMatrix);

  m_subtractor = cv::createBackgroundSubtractorMOG2(m_history, static_cast<double>(m_threshold), m_detectShadows);
}

void Mog2::calculate()
{
  auto const history = std::get<int>(m_inputs[1].value);
  if (m_history != history) {
    m_history = history;
    m_subtractor->setHistory(m_history);
  }

  auto const threshold = std::get<float>(m_inputs[2].value);
  if (!spaghetti::nearly_equal(m_threshold, threshold)) {
    m_threshold = threshold;
    m_subtractor->setVarThreshold(static_cast<double>(m_threshold));
  }

  auto const detectShadows = std::get<bool>(m_inputs[3].value);
  if (m_detectShadows != detectShadows) {
    m_detectShadows = detectShadows;
    m_subtractor->setDetectShadows(m_detectShadows);
  }

  auto matrix = std::get<SafeValue<cv::Mat>>(m_inputs[0].value);
  auto sourceImage = matrix.data();
  cv::Mat foregroundMask{};

  if (!sourceImage.empty() && m_lastFrameTimeStamp != matrix.timeStamp()) {
    m_subtractor->apply(sourceImage, foregroundMask);

    m_outputs[0].value = foregroundMask;
    if (m_outputs.size() == 2) {
      cv::Mat backgroundImage{};
      m_subtractor->getBackgroundImage(backgroundImage);
      m_outputs[1].value = backgroundImage;
    }

    m_lastFrameTimeStamp = matrix.timeStamp();
  }
}

} // namespace spaghetti::elements::opencv
