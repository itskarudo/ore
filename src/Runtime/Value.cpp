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

std::shared_ptr<Object> Value::to_object() const
{
  switch (type()) {
  case Value::Type::Number:
    return std::make_shared<NumberObject>(as_number());
  case Value::Type::Boolean:
    return std::make_shared<BooleanObject>(as_boolean());
  case Value::Type::String:
    return std::make_shared<StringObject>(as_string()->value());
  case Value::Type::Object:
    return std::shared_ptr<Object>(as_object());
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

}
