#include "Cell.h"

namespace Ore::GC {
std::ostream& operator<<(std::ostream& os, Cell const* cell)
{
  return os << cell->class_name() << "@{" << static_cast<void const*>(cell) << "}";
}
}
