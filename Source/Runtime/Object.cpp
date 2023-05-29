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

    auto it = object->m_properties.find(key.string());

    if (it != object->m_properties.end())
      return it->second;

    object = object->shape();
  }

  return interpreter().throw_exception(ExceptionObject::reference_exception(), fmt::format("Unknown identifier '{}'.", key.string()));
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

std::string const Object::to_string() const
{
  return fmt::format("<{}@{}>", class_name(), (void*)this);
}

}
