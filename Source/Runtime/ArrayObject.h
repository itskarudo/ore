#include "Object.h"

namespace Ore {
class ArrayObject final : public Object {
  public:
  ArrayObject(std::vector<Value> elements = {});

  virtual ~ArrayObject() { }

  std::vector<Value>& elements() { return m_elements; }
  std::vector<Value> const& elements() const { return m_elements; }
  virtual char const* class_name() const override { return "ArrayObject"; }
  virtual bool is_array() const override { return true; }

  virtual ThrowResultOr<Value> get(PropertyKey key) const override;
  virtual void put(PropertyKey key, Value value) override;
  virtual bool to_boolean() const override { return !m_elements.empty(); }
  virtual std::string const to_string() const override;

  virtual void visit_graph(Visitor&) override;

  int length() const { return m_elements.size(); }

  private:
  std::vector<Value> m_elements;
};
}
