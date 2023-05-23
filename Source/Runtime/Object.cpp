#include "Object.h"
#include "../Interpreter.h"
#include "NativeFunction.h"
#include <fmt/core.h>

namespace Ore {
void Object::visit_graph(Visitor& visitor)
{
  for (auto [key, value] : properties())
    if (value.is_cell())
      visitor.visit(value.as_cell());
}

ThrowResultOr<Value> Object::get(PropertyKey key) const
{
  if (!key.is_string())
    return interpreter().throw_exception(ExceptionObject::type_exception(), "key must be a string");

  Object const* object = this;
  while (object) {
    if (object->properties().contains(key.string()))
      return object->m_properties.at(key.string());
    object = object->shape();
  }

  return ore_nil();
}

void Object::put(PropertyKey key, Value value)
{
  if (!key.is_string()) {
    interpreter().throw_exception(ExceptionObject::type_exception(), "key must be a string");
    return;
  }

  m_properties[key.string()] = value;
}

void Object::put_native_function(PropertyKey key, std::function<ThrowResultOr<Value>(OreFuncParams)> func)
{
  put(key, heap().allocate<NativeFunction>(func));
}

bool Object::contains(PropertyKey key) const
{
  if (!key.is_string()) {
    interpreter().throw_exception(ExceptionObject::type_exception(), "key must be a string");
    return false;
  }

  Object const* object = this;
  while (object) {
    if (object->properties().contains(key.string()))
      return true;
    object = object->shape();
  }
  return false;
}

std::string const Object::to_string() const
{
  return fmt::format("<{}@{}>", class_name(), (void*)this);
}

}
