#include "Value.h"
#include "ArrayObject.h"
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
    return !as_string()->string().empty();
  case Value::Type::Object:
    return true;
  default:
    return false;
  }
}

Object* Value::to_object(GC::Heap& heap) const
{
  switch (type()) {
  case Value::Type::Number:
    return heap.allocate<NumberObject>(as_number());
  case Value::Type::Boolean:
    return heap.allocate<BooleanObject>(as_boolean());
  case Value::Type::String:
    return heap.allocate<StringObject>(as_string());
  case Value::Type::Object:
    return as_object();
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
    os << value.as_string()->string();
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

Value Value::string_concat(Value const& str1, Value const& str2, GC::Heap& heap)
{
  assert(str1.is_string() && str2.is_string());
  return ore_string(heap, str1.as_string()->string() + str2.as_string()->string());
}

Value Value::logical_and(Value const& v1, Value const& v2)
{
  assert(v1.is_boolean() && v2.is_boolean());
  return Value(v1.as_boolean() && v2.as_boolean());
}

Value Value::logical_or(Value const& v1, Value const& v2)
{
  assert(v1.is_boolean() && v2.is_boolean());
  return Value(v1.as_boolean() || v2.as_boolean());
}

Value Value::logical_xor(Value const& v1, Value const& v2)
{
  assert(v1.is_boolean() && v2.is_boolean());
  return Value(v1.as_boolean() != v2.as_boolean());
}

Value Value::length(Value const& value)
{
  if (value.is_string())
    return Value(static_cast<int>(value.as_string()->string().length()));
  else if (value.is_object() && value.as_object()->is_array()) {
    return Value(static_cast<ArrayObject*>(value.as_object())->length());
  }
  __builtin_unreachable();
}

Value Value::operator+(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() + other.as_number());
}

Value Value::operator-(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() - other.as_number());
}

Value Value::operator*(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() * other.as_number());
}

Value Value::operator/(Value const& other)
{
  assert(is_number() && other.is_number());
  assert(other.as_number() != 0);
  return Value(as_number() / other.as_number());
}

Value Value::operator==(Value const& other)
{
  assert(type() == other.type());

  switch (type()) {
  case Value::Type::Number:
    return Value(as_number() == other.as_number());
  case Value::Type::Boolean:
    return Value(as_boolean() == other.as_boolean());
  case Value::Type::Nil:
    return Value(true);
  case Value::Type::String:
    return Value(as_string()->string() == other.as_string()->string());
  case Value::Type::Object:
    return Value(as_object() == other.as_object());
  default:
    __builtin_unreachable();
  }
}

Value Value::operator!=(Value const& other)
{
  return Value(!(*this == other).as_boolean());
}

Value Value::operator<(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() < other.as_number());
}

Value Value::operator<=(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() <= other.as_number());
}

Value Value::operator>(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() > other.as_number());
}

Value Value::operator>=(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(as_number() >= other.as_number());
}

Value Value::operator<<(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(static_cast<double>((long long)as_number() << (long long)other.as_number()));
}

Value Value::operator>>(Value const& other)
{
  assert(is_number() && other.is_number());
  return Value(static_cast<double>((long long)as_number() >> (long long)other.as_number()));
}

Value Value::operator!(void)
{
  assert(is_boolean());
  return Value(!as_boolean());
}

}
