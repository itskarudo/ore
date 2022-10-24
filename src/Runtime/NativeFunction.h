#pragma once

#include "Object.h"
#include "Value.h"

#include <functional>

namespace Ore {
class NativeFunction final : public Object {
  public:
  explicit NativeFunction(std::function<Value(std::vector<Value>)> native_function)
      : m_native_function(native_function)
  {
  }

  virtual ~NativeFunction() { }

  std::function<Value(std::vector<Value>)>& native_function() { return m_native_function; }

  virtual bool is_native_function() const override { return true; }
  virtual char const* class_name() const override { return "NativeFunction"; }

  private:
  std::function<Value(std::vector<Value>)> m_native_function;
};
}
