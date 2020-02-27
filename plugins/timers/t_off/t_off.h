// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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
#ifndef SPAGHETTI_ELEMENTS_TIMERS_T_OFF_H
#define SPAGHETTI_ELEMENTS_TIMERS_T_OFF_H

#include <chrono>

#include <spaghetti/element.h>

namespace spaghetti::elements {

class TimerOff final : public Element {
 public:
  static constexpr char const *const TYPE{ "Timers/t_off" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  TimerOff();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void update(duration_t const &a_delta) override;
  void calculate() override;

 private:
  enum class State { eWaitForTrigger, eRun, eDone, eReset };

  duration_t m_presetTime{};
  duration_t m_elapsedTime{};
  State m_state{};
  bool m_lastInput{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_TIMERS_T_OFF_H
