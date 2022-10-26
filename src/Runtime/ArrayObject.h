#include "Object.h"

namespace Ore {
class ArrayObject final : public Object {
  public:
  ArrayObject(std::vector<Value> properties = {})
      : m_properties(properties)
  {
  }

  virtual ~ArrayObject() { }

  std::vector<Value> const& properties() const { return m_properties; }
  virtual char const* class_name() const override { return "ArrayObject"; }

  private:
  std::vector<Value> m_properties;
};
}
