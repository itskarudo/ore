#include "ArrayObject.h"
#include <sstream>

namespace Ore {

void ArrayObject::visit_graph(Visitor& visitor)
{
  Object::visit_graph(visitor);

  for (auto& element : m_elements)
    if (element.is_cell())
      element.as_cell()->visit_graph(visitor);
}

Value ArrayObject::get(PropertyKey key) const
{
  if (key.is_number()) {
    assert(key.number() < m_elements.size());
    return m_elements[key.number()];
  }
  if (key.is_string())
    return Object::get(key);

  __builtin_unreachable();
}

void ArrayObject::put(PropertyKey key, Value value)
{
  if (key.is_number())
    m_elements[key.number()] = value;
  else if (key.is_string())
    Object::put(key, value);
}

bool ArrayObject::contains(PropertyKey key) const
{
  if (key.is_number())
    return key.number() < m_elements.size();
  else if (key.is_string())
    return Object::contains(key);

  __builtin_unreachable();
}

std::string const ArrayObject::to_string() const
{
  std::stringstream s;
  s << "[";
  for (auto& element : elements()) {
    s << " " << element << ",";
  }
  s << " ]";

  return s.str();
}

}
