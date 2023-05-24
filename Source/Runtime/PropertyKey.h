#pragma once
#include <string>

namespace Ore {
class PropertyKey {
  public:
  enum class Type {
    Invalid,
    Number,
    String
  };

  PropertyKey() = default;

  PropertyKey(std::string const& value)
      : m_type(Type::String)
      , m_string(value)
  {
  }

  PropertyKey(int value)
      : m_type(Type::Number)
      , m_number(value)
  {
  }

  std::string const& string() const { return m_string; }
  int number() const { return m_number; }

  bool is_string() const { return m_type == Type::String; }
  bool is_number() const { return m_type == Type::Number; }

  private:
  Type m_type { Type::Invalid };
  std::string m_string;
  int m_number;
};
}
