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
    return as_object()->to_boolean();
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
  return ore_boolean(v1.to_boolean() && v2.to_boolean());
}

Value Value::logical_or(Value const& v1, Value const& v2)
{
  return ore_boolean(v1.to_boolean() || v2.to_boolean());
}

Value Value::logical_xor(Value const& v1, Value const& v2)
{
  return ore_boolean(v1.to_boolean() != v2.to_boolean());
}

Value Value::length(Value const& value)
{
  if (value.is_string())
    return ore_number(static_cast<int>(value.as_string()->string().length()));
  else if (value.is_object() && value.as_object()->is_array()) {
    return ore_number(static_cast<ArrayObject*>(value.as_object())->length());
  }
  __builtin_unreachable();
}

Value Value::operator+(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_number(as_number() + other.as_number());
}

Value Value::operator-(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_number(as_number() - other.as_number());
}

Value Value::operator*(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_number(as_number() * other.as_number());
}

Value Value::operator/(Value const& other)
{
  assert(is_number() && other.is_number());
  assert(other.as_number() != 0);
  return ore_number(as_number() / other.as_number());
}

Value Value::operator==(Value const& other)
{
  assert(type() == other.type());

  switch (type()) {
  case Value::Type::Number:
    return ore_boolean(as_number() == other.as_number());
  case Value::Type::Boolean:
    return ore_boolean(as_boolean() == other.as_boolean());
  case Value::Type::Nil:
    return ore_boolean(true);
  case Value::Type::String:
    return ore_boolean(as_string()->string() == other.as_string()->string());
  case Value::Type::Object:
    return ore_boolean(as_object() == other.as_object());
  default:
    __builtin_unreachable();
  }
}

Value Value::operator!=(Value const& other)
{
  return ore_boolean(!(*this == other).as_boolean());
}

Value Value::operator<(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_boolean(as_number() < other.as_number());
}

Value Value::operator<=(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_boolean(as_number() <= other.as_number());
}

Value Value::operator>(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_boolean(as_number() > other.as_number());
}

Value Value::operator>=(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_boolean(as_number() >= other.as_number());
}

Value Value::operator<<(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_number(static_cast<double>((long long)as_number() << (long long)other.as_number()));
}

Value Value::operator>>(Value const& other)
{
  assert(is_number() && other.is_number());
  return ore_number(static_cast<double>((long long)as_number() >> (long long)other.as_number()));
}

Value Value::operator!(void)
{
  return ore_boolean(!to_boolean());
}

}
