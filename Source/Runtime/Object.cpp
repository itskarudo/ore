#include "Object.h"
#include "NativeFunction.h"
#include <sstream>

namespace Ore {
void Object::visit_graph(Visitor& visitor)
{
  Cell::visit_graph(visitor);

  for (auto [key, value] : properties())
    if (value.is_cell())
      value.as_cell()->visit_graph(visitor);
}

Value Object::get(PropertyKey key) const
{
  assert(key.is_string());
  return m_properties.at(key.string());
}

void Object::put(PropertyKey key, Value value)
{
  assert(key.is_string());
  m_properties[key.string()] = value;
}

void Object::put_native_function(PropertyKey key, std::function<Value(std::vector<Value>)> func)
{
  put(key, heap().allocate<NativeFunction>(func));
}

bool Object::contains(PropertyKey key) const
{
  assert(key.is_string());
  return m_properties.count(key.string());
}

std::string const Object::to_string() const
{
  std::stringstream s;
  s << "{";
  for (auto& [key, value] : properties()) {
    s << " \"" << key << "\": ";
    if (value.is_string())
      s << "\"" << value << "\",";
    else
      s << value << ",";
  }
  s << " }";
  return s.str();
}

}
