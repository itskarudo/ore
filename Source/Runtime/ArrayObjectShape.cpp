#include "ArrayObjectShape.h"
#include "../Interpreter.h"
#include "ArrayObject.h"
#include "ExceptionObject.h"
#include "FunctionObject.h"

namespace Ore {
ArrayObjectShape::ArrayObjectShape()
{
  REGISTER_NATIVE_FUNCTION(append);
  REGISTER_NATIVE_FUNCTION(pop);
  REGISTER_NATIVE_FUNCTION(insert);
  REGISTER_NATIVE_FUNCTION(remove);
  REGISTER_NATIVE_FUNCTION(map);
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::append)
{
  ARGS_SIZE_GUARD(append, 2);
  ARG_TYPE_ARRAY(0);

  auto* self = static_cast<ArrayObject*>(params.args[0].as_object());

  self->elements().push_back(params.args[1]);

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::pop)
{
  ARGS_SIZE_GUARD(pop, 1);
  ARG_TYPE_ARRAY(0);

  auto* self = static_cast<ArrayObject*>(params.args[0].as_object());
  auto value = self->elements().back();

  self->elements().pop_back();

  return value;
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::insert)
{
  ARGS_SIZE_GUARD(insert, 3);
  ARG_TYPE_ARRAY(0);
  ARG_TYPE_NUMBER(1);

  auto* self = static_cast<ArrayObject*>(params.args[0].as_object());

  auto index = params.args[1].as_number();
  auto value = params.args[2];

  self->elements().insert(self->elements().begin() + index, value);

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::remove)
{
  ARGS_SIZE_GUARD(remove, 2);
  ARG_TYPE_ARRAY(0);
  ARG_TYPE_NUMBER(1);

  auto* self = static_cast<ArrayObject*>(params.args[0].as_object());

  auto index = params.args[1].as_number();

  self->elements().erase(self->elements().begin() + index);

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::map)
{
  ARGS_SIZE_GUARD(map, 2);
  ARG_TYPE_ARRAY(0);
  ARG_TYPE_FUNCTION(1);

  auto* self = static_cast<ArrayObject*>(params.args[0].as_object());
  auto* callback = static_cast<FunctionObject*>(params.args[1].as_object());

  std::vector<Value> results(self->elements().size());

  if (callback->parameters().size() != 2)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "Callback function takes exactly 2 parameters.");

  for (size_t i = 0; i < self->elements().size(); i++) {
    std::map<std::string, Value> arguments;
    arguments[callback->parameters()[0].name] = self->elements()[i];
    arguments[callback->parameters()[1].name] = ore_number(i);

    auto result = TRY(params.interpreter.run(*callback->body(), arguments, Interpreter::ScopeType::Function, callback->name()));

    results[i] = result;
  }

  return Value(params.interpreter.heap().allocate<ArrayObject>(std::move(results)));
}
}
