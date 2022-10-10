#include "Value.h"
#include "BooleanObject.h"
#include "NumberObject.h"
#include "Object.h"
#include "StringObject.h"

namespace Ore {
bool Value::to_boolean() const
{
  switch (type()) {
  case Value::Type::Number:
    return as_number() != 0;
  case Value::Type::Boolean:
    return as_boolean();
  case Value::Type::String:
    return !as_string()->value().empty();
  case Value::Type::Object:
    return true;
  default:
    return false;
  }
}

Value Value::to_object(GC::Heap& heap) const
{
  switch (type()) {
  case Value::Type::Number:
    return Value(heap.allocate<NumberObject>(as_number()));
  case Value::Type::Boolean:
    return Value(heap.allocate<BooleanObject>(as_boolean()));
  case Value::Type::String:
    return Value(heap.allocate<StringObject>(as_string()));
  case Value::Type::Object:
    return Value(as_object());
  default:
    __builtin_unreachable();
  }
}

std::ostream& operator<<(std::ostream& os, Value const& value)
{
  switch (value.type()) {
  case Value::Type::Number:
    os << value.as_number();
    break;
  case Value::Type::Boolean:
    os << (value.as_boolean() ? "true" : "false");
    break;
  case Value::Type::String:
    os << value.as_string()->value();
    break;
  case Value::Type::Object:
    os << "[Object]";
    break;
  case Value::Type::Nil:
    os << "nil";
    break;
  }

  return os;
}

Value add(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());

  return Value(v1.as_number() + v2.as_number());
}

Value sub(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());

  return Value(v1.as_number() - v2.as_number());
}
Value multiply(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());

  return Value(v1.as_number() * v2.as_number());
}
Value divide(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());
  assert(v2.as_number() != 0);

  return Value(v1.as_number() / v2.as_number());
}
Value equals(Value v1, Value v2)
{
  assert(v1.type() == v2.type());
  switch (v1.type()) {
  case Value::Type::Number:
    return Value(v1.as_number() == v2.as_number());
  case Value::Type::Boolean:
    return Value(v1.as_boolean() == v2.as_boolean());
  case Value::Type::Nil:
    return Value(true);
  case Value::Type::String:
    return Value(v1.as_string()->value() == v2.as_string()->value());
  case Value::Type::Object:
    return Value(v1.as_object() == v2.as_object());
  default:
    __builtin_unreachable();
  }
}
Value not_equals(Value v1, Value v2)
{
  assert(v1.type() == v2.type());
  switch (v1.type()) {
  case Value::Type::Number:
    return Value(v1.as_number() != v2.as_number());
  case Value::Type::Boolean:
    return Value(v1.as_boolean() != v2.as_boolean());
  case Value::Type::Nil:
    return Value(false);
  case Value::Type::String:
    return Value(v1.as_string()->value() != v2.as_string()->value());
  case Value::Type::Object:
    return Value(v1.as_object() != v2.as_object());
  default:
    __builtin_unreachable();
  }
}

Value greater_than(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());
  return Value(v1.as_number() > v2.as_number());
}
Value less_than(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());
  return Value(v1.as_number() < v2.as_number());
}
Value greater_than_or_equals(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());
  return Value(v1.as_number() >= v2.as_number());
}
Value less_than_or_equals(Value v1, Value v2)
{
  assert(v1.is_number());
  assert(v2.is_number());
  return Value(v1.as_number() <= v2.as_number());
}

Value string_concat(Value v1, Value v2, GC::Heap& heap)
{
  assert(v1.is_string());
  assert(v2.is_string());

  auto str1 = v1.as_string()->value();
  auto str2 = v2.as_string()->value();

  auto* str = heap.allocate<PrimitiveString>(str1 + str2);

  return Value(str);
}

}
