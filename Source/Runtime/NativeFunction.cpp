#include "NativeFunction.h"
#include <fmt/core.h>

namespace Ore {
std::string const NativeFunction::to_string() const
{
  return fmt::format("<NativeFunction@{}>", (void*)this);
}
}
