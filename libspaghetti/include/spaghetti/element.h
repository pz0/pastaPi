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
#ifndef SPAGHETTI_ELEMENT_H
#define SPAGHETTI_ELEMENT_H

// clang-format off
#ifdef _MSC_VER
# pragma warning(disable:4251)
# pragma warning(disable:4307)
#endif
// clang-format on

#include <chrono>
#include <functional>
#include <set>
#include <string>
#include <thread>
#include <variant>
#include <vector>
#include <mutex>

#include <spaghetti/vendor/json.hpp>

#include <spaghetti/api.h>
#include <spaghetti/strings.h>
#include <spaghetti/sockvalues.h>

namespace spaghetti {

class Package;

struct EventNameChanged {
  std::string from;
  std::string to;
};
struct EventIONameChanged {
  std::string from;
  std::string to;
  uint64_t id;
  bool input;
};
struct EventIOTypeChanged {
  bool input;
  uint64_t id;
  ValueType from;
  ValueType to;
};
struct EventEmpty {
};
using EventValue = std::variant<EventNameChanged, EventIONameChanged, EventIOTypeChanged, EventEmpty>;
enum class EventType {
  eElementNameChanged,
  eIONameChanged,
  eIOTypeChanged,
  eInputAdded,
  eInputRemoved,
  eOutputAdded,
  eOutputRemoved
};

struct Event {
  EventType type{};
  EventValue payload{};
};

using EventCallback = std::function<void(Event const &)>;

class SPAGHETTI_API Element {
 public:
  using duration_t = std::chrono::duration<double, std::milli>;

  using Json = nlohmann::json;

  template<typename T>
  struct Vec2 {
    T x{};
    T y{};
  };
  using vec2 = Vec2<int32_t>;
  using vec2f = Vec2<float>;
  using vec2d = Vec2<double>;

  struct IOSocket : public IOSocketFlags {
    using TimeStamp = std::chrono::high_resolution_clock::time_point;

    IOSocket()
      : writeLock{ new std::mutex{} }
    {
    }

   public:
    ValueType type{};
    size_t id{};
    uint64_t slot{};
    uint64_t flags{};
    std::string name{};

    bool valueChanged{ true };
    bool isMonitored{ false };
    TimeStamp timeStamp{};

    template<typename T>
    T getValue()
    {
      T outValue;

      if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eProtectedValuesFlags)) {
        writeLock->lock();
        outValue = std::get<T>(value);
        writeLock->unlock();
      } else {
        outValue = std::get<T>(value);
      }

      valueChanged = false;
      return outValue;
    }

    template<typename T>
    T getValueWithoutNotify() const
    {
      T outValue;

      if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eProtectedValuesFlags)) {
        writeLock->lock();
        outValue = std::get<T>(value);
        writeLock->unlock();
      } else {
        outValue = std::get<T>(value);
      }

      return outValue;
    }

    template<typename T>
    void setValue(T const a_value)
    {
      if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eTimeStampedValues))
        timeStamp = std::chrono::high_resolution_clock::now();

      if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eProtectedValuesFlags)) {
        writeLock->lock();
        value = a_value;
        writeLock->unlock();
      } else {
        value = a_value;
      }
    }

    void copyValue(IOSocket const &a_from)
    {
      if (isMonitored) {
        if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eTimeStampedValues)) {
          if (timeStamp == a_from.timeStamp) {
            valueChanged = false;
          } else {
            timeStamp = a_from.timeStamp;
            valueChanged = true;
          }

        } else {
          valueChanged = !ValueDescription::compareValues(value, a_from.value, type);
        }
      }

      if (ValueDescription::isTypeAlowed(type, IOSocketFlags::eProtectedValuesFlags)) {
        writeLock->lock();
        value = a_from.value;
        writeLock->unlock();
      } else {
        value = a_from.value;
      }
    }

   private:
    Value value{};
    std::shared_ptr<std::mutex> writeLock{};
  };

  using IOSockets = std::vector<IOSocket>;

  Element() = default;
  virtual ~Element() = default;

  virtual char const *type() const noexcept = 0;
  virtual string::hash_t hash() const noexcept = 0;

  virtual void serialize(Json &a_json);
  virtual void deserialize(Json const &a_json);

  virtual void calculate() {}
  virtual void reset() {}
  virtual bool alwaysCalculate() const noexcept { return true; }

  virtual void update(duration_t const &a_delta) { (void)a_delta; }

  size_t id() const noexcept { return m_id; }

  void setName(std::string const &a_name);

  std::string name() const noexcept { return m_name; }

  void setPosition(double const a_x, double const a_y)
  {
    m_position.x = a_x;
    m_position.y = a_y;
  }
  void setPosition(vec2d const &a_position) { m_position = a_position; }
  vec2d const &position() const { return m_position; }

  void iconify(bool const a_iconify) { m_isIconified = a_iconify; }
  bool isIconified() const { return m_isIconified; }

  void setIconifyingHidesCentralWidget(bool const a_hide) { m_iconifyingHidesCentralWidget = a_hide; }
  bool iconifyingHidesCentralWidget() const { return m_iconifyingHidesCentralWidget; }

  IOSockets &inputs() { return m_inputs; }
  IOSockets const &inputs() const { return m_inputs; }
  IOSockets &outputs() { return m_outputs; }
  IOSockets const &outputs() const { return m_outputs; }

  bool addInput(ValueType const a_type, std::string const &a_name, uint64_t const a_flags);
  void setInputName(uint64_t const a_input, std::string const &a_name);
  void removeInput();
  void clearInputs();

  bool addOutput(ValueType const a_type, std::string const &a_name, uint64_t const a_flags);
  void setOutputName(uint64_t const a_output, std::string const &a_name);
  void removeOutput();
  void clearOutputs();

  virtual bool hasInvertedIO() { return false; }

  void setIOName(bool const a_input, uint64_t const a_id, std::string const &a_name);
  void setIOValueType(bool const a_input, uint64_t const a_id, ValueType const a_type);

  bool connect(size_t const a_sourceId, uint64_t const a_outputId, uint64_t const a_inputId);

  uint64_t minInputs() const { return m_minInputs; }
  uint64_t maxInputs() const { return m_maxInputs; }
  uint64_t defaultNewInputFlags() const { return m_defaultNewInputFlags; }
  uint64_t minOutputs() const { return m_minOutputs; }
  uint64_t maxOutputs() const { return m_maxOutputs; }
  uint64_t defaultNewOutputFlags() const { return m_defaultNewOutputFlags; }

  Package *package() const { return m_package; }

  void resetIOSocketValue(IOSocket &a_io);

  void setNode(void *const a_node) { m_node = a_node; }

  template<typename T>
  T *node()
  {
    return static_cast<T *>(m_node);
  }

  void registerEventHandler(EventCallback const &a_handler) { m_handler = a_handler; }

 protected:
  void handleEvent(Event const &a_event);
  virtual void onEvent(Event const &a_event) { (void)a_event; }

  void setMinInputs(uint64_t const a_min);
  void setMaxInputs(uint64_t const a_max);
  void setDefaultNewInputFlags(uint64_t const a_flags) { m_defaultNewInputFlags = a_flags; }

  void setMinOutputs(uint64_t const a_min);
  void setMaxOutputs(uint64_t const a_max);
  void setDefaultNewOutputFlags(uint64_t const a_flags) { m_defaultNewOutputFlags = a_flags; }

 protected:
  IOSockets m_inputs{};
  IOSockets m_outputs{};

  friend class Package;
  Package *m_package{};

 private:
  size_t m_id{};
  std::string m_name{};
  vec2d m_position{};
  bool m_isIconified{};
  bool m_iconifyingHidesCentralWidget{};
  uint8_t m_minInputs{};
  uint8_t m_maxInputs{ std::numeric_limits<uint8_t>::max() };
  uint8_t m_minOutputs{};
  uint8_t m_maxOutputs{ std::numeric_limits<uint8_t>::max() };
  uint64_t m_defaultNewInputFlags{};
  uint64_t m_defaultNewOutputFlags{};
  EventCallback m_handler{};
  void *m_node{};
};

template<typename T>
inline void to_json(Element::Json &a_json, Element::Vec2<T> const &a_value)
{
  a_json = Element::Json{ a_value.x, a_value.y };
}

template<typename T>
inline void from_json(Element::Json const &a_json, Element::Vec2<T> &a_value)
{
  a_value.x = a_json[0].get<T>();
  a_value.y = a_json[1].get<T>();
}

} // namespace spaghetti

#endif // SPAGHETTI_ELEMENT_H
