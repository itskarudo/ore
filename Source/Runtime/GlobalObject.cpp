#include "GlobalObject.h"
#include "../Interpreter.h"
#include <fmt/core.h>

namespace Ore {
GlobalObject::GlobalObject()
{
  set_shape(interpreter().global_object_shape());
}

ThrowResultOr<Value> GlobalObject::get(PropertyKey key) const
{
  if (!key.is_string())
    return interpreter().throw_exception(ExceptionObject::type_exception(), "key must be a string");

  Object const* object = this;
  while (object) {

    auto it = object->properties().find(key.string());

    if (it != object->properties().end())
      return it->second;

    object = object->shape();
  }

  return interpreter().throw_exception(ExceptionObject::reference_exception(), fmt::format("Unknown identifier '{}'.", key.string()));
}

bool GlobalObject::contains(PropertyKey key) const
{
  assert(key.is_string());

  Object const* object = this;
  while (object) {

    auto it = object->properties().find(key.string());

    if (it != object->properties().end())
      return true;

    object = object->shape();
  }

  return false;
}

}
