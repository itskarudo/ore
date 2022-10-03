#include "StringObject.h"

namespace Ore {
void StringObject::visit_graph(Visitor& visitor)
{
  GC::Cell::visit_graph(visitor);
  value()->visit_graph(visitor);
}
}
