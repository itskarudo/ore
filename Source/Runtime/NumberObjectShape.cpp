#include "NumberObjectShape.h"

#include "../Interpreter.h"
#include "NumberObject.h"

namespace Ore {
NumberObjectShape::NumberObjectShape()
{
  REGISTER_NATIVE_FUNCTION(to_string);
}

DEFINE_NATIVE_FUNCTION(NumberObjectShape::to_string)
{
  ARGS_SIZE_GUARD(to_string, 1);
  ARG_TYPE_OBJECT(0);

  auto* self = static_cast<NumberObject*>(params.args[0].as_object());

  auto string_value = std::to_string(self->value());

  return ore_string(params.interpreter.heap(), string_value);
}

}
