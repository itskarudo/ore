#include "Value.h"
#include "../Interpreter.h"
#include "ArrayObject.h"
#include "BooleanObject.h"
#include "ExceptionObject.h"
#include "NumberObject.h"
#include "Object.h"
#include "Result.h"
#include "StringObject.h"
#include <math.h>

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
    ASSERT_NOT_REACHED();
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
    os << value.as_object()->to_string();
    break;
  case Value::Type::Nil:
    os << "nil";
    break;
  }

  return os;
}

ThrowResultOr<Value> Value::string_concat(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_string())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a string");
  if (!rhs.is_string())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a string");

  return ore_string(interpreter.heap(), lhs.as_string()->string() + rhs.as_string()->string());
}

ThrowResultOr<Value> Value::modulo(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(lhs.as_number() - rhs.as_number() * static_cast<int>(lhs.as_number() / rhs.as_number()));
}

ThrowResultOr<Value> Value::logical_and(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.to_boolean())
    return lhs;
  else
    return rhs;
}

ThrowResultOr<Value> Value::logical_or(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (lhs.to_boolean())
    return lhs;
  else
    return rhs;
}

ThrowResultOr<Value> Value::logical_xor(Interpreter& interpreter, Value lhs, Value rhs)
{
  return ore_boolean(lhs.to_boolean() != rhs.to_boolean());
}

ThrowResultOr<Value> Value::logical_not(Interpreter& interpreter, Value operand)
{
  return ore_boolean(!operand.to_boolean());
}

ThrowResultOr<Value> Value::length(Interpreter& interpreter, Value operand)
{
  if (operand.is_string())
    return ore_number(static_cast<int>(operand.as_string()->string().length()));
  else if (operand.is_object() && operand.as_object()->is_array()) {
    return ore_number(static_cast<ArrayObject*>(operand.as_object())->length());
  }

  return interpreter.throw_exception(ExceptionObject::type_exception(), "Object has no length property");
}

ThrowResultOr<Value> Value::negate(Interpreter& interpreter, Value operand)
{
  if (!operand.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "operand is not a number");

  return ore_number(-operand.as_number());
}

ThrowResultOr<Value> Value::add(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(lhs.as_number() + rhs.as_number());
}

ThrowResultOr<Value> Value::subtract(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(lhs.as_number() - rhs.as_number());
}

ThrowResultOr<Value> Value::multiply(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(lhs.as_number() * rhs.as_number());
}

ThrowResultOr<Value> Value::divide(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  if (rhs.as_number() == 0)
    return interpreter.throw_exception(ExceptionObject::division_by_zero_exception(), "Cannot divide by zero");

  return ore_number(lhs.as_number() / rhs.as_number());
}

ThrowResultOr<Value> Value::pow(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(std::pow(lhs.as_number(), rhs.as_number()));
}

ThrowResultOr<Value> Value::equals(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (lhs.type() != rhs.type())
    return ore_boolean(false);

  switch (lhs.type()) {
  case Value::Type::Number:
    return ore_boolean(lhs.as_number() == rhs.as_number());
  case Value::Type::Boolean:
    return ore_boolean(lhs.as_boolean() == rhs.as_boolean());
  case Value::Type::Nil:
    return ore_boolean(true);
  case Value::Type::String:
    return ore_boolean(lhs.as_string()->string() == rhs.as_string()->string());
  case Value::Type::Object:
    return ore_boolean(lhs.as_object() == rhs.as_object());
  default:
    ASSERT_NOT_REACHED();
  }
}

ThrowResultOr<Value> Value::not_equals(Interpreter& interpreter, Value lhs, Value rhs)
{
  return ore_boolean(!TRY(Value::equals(interpreter, lhs, rhs)).to_boolean());
}

ThrowResultOr<Value> Value::less_than(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_boolean(lhs.as_number() < rhs.as_number());
}

ThrowResultOr<Value> Value::less_than_or_equals(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_boolean(lhs.as_number() <= rhs.as_number());
}

ThrowResultOr<Value> Value::greater_than(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_boolean(lhs.as_number() > rhs.as_number());
}

ThrowResultOr<Value> Value::greater_than_or_equals(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_boolean(lhs.as_number() >= rhs.as_number());
}

ThrowResultOr<Value> Value::shift_left(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(static_cast<double>((long long)lhs.as_number() << (long long)rhs.as_number()));
}

ThrowResultOr<Value> Value::shift_right(Interpreter& interpreter, Value lhs, Value rhs)
{
  if (!lhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "lhs is not a number");
  if (!rhs.is_number())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "rhs is not a number");

  return ore_number(static_cast<double>((long long)lhs.as_number() >> (long long)rhs.as_number()));
}

}
