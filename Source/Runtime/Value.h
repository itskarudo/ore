#pragma once

#include "../Forward.h"
#include "../GC/Heap.h"
#include "PrimitiveString.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace Ore {
class Value {
  public:
  enum class Type {
    Number,
    Boolean,
    String,
    Object,
    Nil
  };

  Value(double value)
      : m_type(Type::Number)
  {
    m_value.as_number = value;
  }

  Value(int value)
      : m_type(Type::Number)
  {
    m_value.as_number = value;
  }

  Value(bool value)
      : m_type(Type::Boolean)
  {
    m_value.as_boolean = value;
  }

  Value(PrimitiveString* value)
      : m_type(Type::String)
  {
    m_value.as_string = value;
  }

  Value(Object* value)
      : m_type(Type::Object)
  {
    m_value.as_object = value;
  }

  explicit Value()
      : m_type(Type::Nil)
  {
  }

  Value(Type type)
      : m_type(type)
  {
  }

  Type type() const { return m_type; }

  bool is_number() const { return type() == Type::Number; }
  bool is_string() const { return type() == Type::String; }
  bool is_object() const { return type() == Type::Object; }
  bool is_boolean() const { return type() == Type::Boolean; }
  bool is_nil() const { return type() == Type::Nil; }

  bool is_cell() const { return is_string() || is_object(); }

  double as_number() const
  {
    assert(is_number());
    return m_value.as_number;
  }

  bool as_boolean() const
  {
    assert(is_boolean());
    return m_value.as_boolean;
  }

  PrimitiveString* as_string() const
  {
    assert(is_string());
    return m_value.as_string;
  }

  Object* as_object() const
  {
    assert(is_object());
    return m_value.as_object;
  }

  GC::Cell* as_cell() const
  {
    assert(is_cell());
    return m_value.as_cell;
  }

  bool to_boolean() const;
  Object* to_object(GC::Heap&) const;

  friend std::ostream& operator<<(std::ostream& os, Value const& value);

  static ThrowResultOr<Value> string_concat(Interpreter&, Value, Value);
  static ThrowResultOr<Value> logical_and(Interpreter&, Value, Value);
  static ThrowResultOr<Value> logical_or(Interpreter&, Value, Value);
  static ThrowResultOr<Value> logical_xor(Interpreter&, Value, Value);
  static ThrowResultOr<Value> logical_not(Interpreter&, Value);
  static ThrowResultOr<Value> length(Interpreter&, Value);
  static ThrowResultOr<Value> negate(Interpreter&, Value);
  static ThrowResultOr<Value> add(Interpreter&, Value, Value);
  static ThrowResultOr<Value> subtract(Interpreter&, Value, Value);
  static ThrowResultOr<Value> multiply(Interpreter&, Value, Value);
  static ThrowResultOr<Value> divide(Interpreter&, Value, Value);
  static ThrowResultOr<Value> pow(Interpreter&, Value, Value);
  static ThrowResultOr<Value> equals(Interpreter&, Value, Value);
  static ThrowResultOr<Value> not_equals(Interpreter&, Value, Value);
  static ThrowResultOr<Value> less_than(Interpreter&, Value, Value);
  static ThrowResultOr<Value> less_than_or_equals(Interpreter&, Value, Value);
  static ThrowResultOr<Value> greater_than(Interpreter&, Value, Value);
  static ThrowResultOr<Value> greater_than_or_equals(Interpreter&, Value, Value);
  static ThrowResultOr<Value> shift_right(Interpreter&, Value, Value);
  static ThrowResultOr<Value> shift_left(Interpreter&, Value, Value);

  private:
  Type m_type;
  union {
    double as_number;
    bool as_boolean;
    PrimitiveString* as_string;
    Object* as_object;
    GC::Cell* as_cell;
  } m_value;
};

inline Value ore_number(double value)
{
  return Value(value);
}

inline Value ore_boolean(bool value)
{
  return Value(value);
}

inline Value ore_string(GC::Heap& heap, std::string const& string)
{
  return heap.allocate<PrimitiveString>(string);
}

inline Value ore_object(GC::Heap& heap)
{
  return heap.allocate<Object>();
}

inline Value ore_nil()
{
  return Value(Value::Type::Nil);
}

}
