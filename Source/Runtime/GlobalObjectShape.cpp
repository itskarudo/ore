#include "GlobalObjectShape.h"
#include "../Interpreter.h"
#include "ArrayObject.h"
#include "ExceptionObject.h"

namespace Ore {
GlobalObjectShape::GlobalObjectShape()
{
  REGISTER_NATIVE_FUNCTION(gc);
  REGISTER_NATIVE_FUNCTION(assert);
  REGISTER_NATIVE_FUNCTION(throw);
  REGISTER_NATIVE_FUNCTION(all);
  REGISTER_NATIVE_FUNCTION(any);
  REGISTER_NATIVE_FUNCTION(type);
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
  ARG_TYPE_ARRAY(0);

  auto* array = static_cast<ArrayObject*>(params.args[0].as_object());

  for (auto element : array->elements())
    if (!element.to_boolean())
      return ore_boolean(false);

  return ore_boolean(true);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::any)
{
  ARGS_SIZE_GUARD(any, 1);
  ARG_TYPE_ARRAY(0);

  auto* array = static_cast<ArrayObject*>(params.args[0].as_object());

  for (auto element : array->elements())
    if (element.to_boolean())
      return ore_boolean(true);

  return ore_boolean(false);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::type)
{
  ARGS_SIZE_GUARD(type, 1);
  return ore_string(params.interpreter.heap(), params.args[0].type_name());
}

}
