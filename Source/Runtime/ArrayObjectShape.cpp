#include "ArrayObjectShape.h"
#include "ArrayObject.h"

namespace Ore {
ArrayObjectShape::ArrayObjectShape()
{
  REGISTER_NATIVE_FUNCTION(append);
}

DEFINE_NATIVE_FUNCTION(ArrayObjectShape::append)
{
  assert(args.size() == 2);

  assert(args[0].is_object());
  auto* self_object = args[0].as_object();

  assert(self_object->is_array());
  auto* self = static_cast<ArrayObject*>(self_object);

  self->elements().push_back(args[1]);

  return ore_nil();
}
}
