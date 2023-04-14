#pragma once

#include "Value.h"
#include <optional>

namespace Ore {
class Result {
  public:
  enum class Type {
    None,
    Normal,
    Return,
    Throw,
    Break,
    Continue
  };

  Result(Type type, std::optional<Value> value)
      : m_type(type)
      , m_value(value)
  {
  }

  Result(Value value)
      : Result(Type::Normal, value)
  {
  }

  Result()
      : Result(ore_nil())
  {
  }

  Type type() const { return m_type; }
  bool has_value() const { return m_value.has_value(); }
  Value value() const { return m_value.value(); }

  bool is_exception() const { return m_type == Type::Throw; }

  private:
  Type m_type { Type::None };
  std::optional<Value> m_value;
};
}

#define TRY(expr)                                                              \
  ({                                                                           \
    auto&& _tmp_result = (expr);                                               \
    static_assert(!std::is_lvalue_reference_v<decltype(_tmp_result.value())>); \
    if (_tmp_result.is_exception()) [[unlikely]]                               \
      return _tmp_result;                                                      \
    _tmp_result.value();                                                       \
  })
