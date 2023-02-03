#include "GlobalObject.h"
#include "../Interpreter.h"

namespace Ore {
GlobalObject::GlobalObject()
{
  set_shape(interpreter().global_object_shape());
}

}
