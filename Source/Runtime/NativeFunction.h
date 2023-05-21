#pragma once

#include "Object.h"
#include "Value.h"

#include <functional>

namespace Ore {
class NativeFunction final : public Object {
  public:
  explicit NativeFunction(std::function<ThrowResultOr<Value>(OreFuncParams)> native_function)
      : m_native_function(native_function)
  {
  }

  virtual ~NativeFunction() { }

  std::function<ThrowResultOr<Value>(OreFuncParams)>& native_function() { return m_native_function; }

  virtual bool is_native_function() const override { return true; }
  virtual char const* class_name() const override { return "NativeFunction"; }
  virtual std::string const to_string() const override;

  private:
  std::function<ThrowResultOr<Value>(OreFuncParams)> m_native_function;
};
}
