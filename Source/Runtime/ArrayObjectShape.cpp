#include "ArrayObjectShape.h"
#include "../Interpreter.h"
#include "ArrayObject.h"
#include "ExceptionObject.h"

namespace Ore {
ArrayObjectShape::ArrayObjectShape()
{
  REGISTER_NATIVE_FUNCTION(append);
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::append)
{
  ARGS_SIZE_GUARD(append, 2);
  ARG_TYPE_OBJECT(0);

  auto* self_object = params.args[0].as_object();

  assert(self_object->is_array());
  auto* self = static_cast<ArrayObject*>(self_object);

  self->elements().push_back(params.args[1]);

  return ore_nil();
}
}
