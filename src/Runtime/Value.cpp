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

}
