#include "GlobalObjectShape.h"
#include "ExceptionObject.h"
#include "FFIObject.h"

namespace Ore {
GlobalObjectShape::GlobalObjectShape()
{
  REGISTER_NATIVE_FUNCTION(gc);
  REGISTER_NATIVE_FUNCTION(assert);
  REGISTER_NATIVE_FUNCTION(throw);
  REGISTER_NATIVE_FUNCTION(all);
  REGISTER_NATIVE_FUNCTION(any);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::gc)
{
  ARGS_SIZE_GUARD(print, 0);

  params.interpreter.heap().collect_garbage();
  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::assert)
{
  ARGS_SIZE_GUARD(assert, 1);

  if (!params.args[0].to_boolean())
    return params.interpreter.throw_exception(ExceptionObject::assertion_exception(), "assertion failed");

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::throw)
{
  ARGS_SIZE_GUARD(throw, 2);
  ARG_TYPE_STRING(0);
  ARG_TYPE_STRING(1);

  return params.interpreter.throw_exception(params.args[0].as_string()->string(), params.args[1].as_string()->string());
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::all)
{
  ARGS_SIZE_GUARD(all, 1);
  ARG_TYPE_OBJECT(0);

  auto* self_object = params.args[0].as_object();
  assert(self_object->is_array());

  auto* self = static_cast<ArrayObject*>(self_object);

  for (auto element : self->elements())
    if (!element.to_boolean())
      return ore_boolean(false);

  return ore_boolean(true);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::any)
{
  ARGS_SIZE_GUARD(any, 1);
  ARG_TYPE_OBJECT(0);

  auto* self_object = params.args[0].as_object();
  assert(self_object->is_array());

  auto* self = static_cast<ArrayObject*>(self_object);

  for (auto element : self->elements())
    if (element.to_boolean())
      return ore_boolean(true);

  return ore_boolean(false);
}

}
