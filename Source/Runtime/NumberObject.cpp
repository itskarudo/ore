#include "NumberObject.h"
#include "../Interpreter.h"

namespace Ore {

NumberObject::NumberObject(double value)
    : m_value(value)
{
  set_shape(interpreter().number_object_shape());
}

}
