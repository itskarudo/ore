#include "ArrayObject.h"
#include "../Interpreter.h"
#include "fmt/format.h"
#include <sstream>

namespace Ore {

ArrayObject::ArrayObject(std::vector<Value> elements)
    : m_elements(elements)
{
  set_shape(interpreter().array_object_shape());
}

void ArrayObject::visit_graph(Visitor& visitor)
{
  Object::visit_graph(visitor);

  for (auto& element : m_elements)
    if (element.is_cell())
      visitor.visit(element.as_cell());
}

Value ArrayObject::get(PropertyKey key) const
{
  if (key.is_number()) {
    if (key.number() >> m_elements.size())
      return interpreter().throw_exception(ExceptionObject::out_of_bounds_index_exception(), fmt::format("{} is out of bounds", key.number()));
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
