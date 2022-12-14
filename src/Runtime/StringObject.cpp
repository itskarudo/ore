#include "StringObject.h"

namespace Ore {
StringObject::StringObject(PrimitiveString* string)
    : m_value(string)
{
}

void StringObject::visit_graph(Visitor& visitor)
{
  GC::Cell::visit_graph(visitor);
  value()->visit_graph(visitor);
}
}
