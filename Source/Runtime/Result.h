#pragma once

#include "Value.h"
#include <optional>
#include <variant>

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
  Result exception() const
  {
    assert(is_exception());
    assert(has_value());
    return { m_type, m_value };
  }

  private:
  Type m_type { Type::None };
  std::optional<Value> m_value;
};

template<typename T>
class ThrowResultOr {
  public:
  ThrowResultOr(T value)
      : m_value_or_throw_result(std::move(value))
  {
  }
  ThrowResultOr(Result throw_result)
  {
    assert(throw_result.is_exception());
    m_value_or_throw_result = std::move(throw_result);
  }

  T value() const { return std::get<T>(m_value_or_throw_result); }

  bool is_exception() const { return std::holds_alternative<Result>(m_value_or_throw_result); }
  Result exception() const { return std::get<Result>(m_value_or_throw_result); }

  private:
  std::variant<T, Result> m_value_or_throw_result;
};

}

#define TRY(expr)                                                              \
  ({                                                                           \
    auto&& _tmp_result = (expr);                                               \
    static_assert(!std::is_lvalue_reference_v<decltype(_tmp_result.value())>); \
    if (_tmp_result.is_exception()) [[unlikely]]                               \
      return _tmp_result.exception();                                          \
    _tmp_result.value();                                                       \
  })
