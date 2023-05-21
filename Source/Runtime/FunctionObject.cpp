#include "FunctionObject.h"
#include <fmt/core.h>

namespace Ore {

std::string const FunctionObject::to_string() const
{
  return fmt::format("<Function@{}>", (void*)this);
}
}
