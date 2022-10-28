#include "Object.h"

namespace Ore {
class ArrayObject final : public Object {
  public:
  ArrayObject(std::vector<Value> elements = {})
      : m_elements(elements)
  {
  }

  virtual ~ArrayObject() { }

  std::vector<Value> const& elements() const { return m_elements; }
  virtual char const* class_name() const override { return "ArrayObject"; }
  virtual bool is_array() const override { return true; }

  virtual Value get(PropertyKey key) const override;
  virtual void put(PropertyKey key, Value value) override;
  virtual bool contains(PropertyKey key) const override;

  int length() const { return m_elements.size(); }

  private:
  std::vector<Value> m_elements;
};
}
