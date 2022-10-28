#include "ArrayObject.h"

namespace Ore {

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

}
