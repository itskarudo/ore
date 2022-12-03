#pragma once

#include "../GC/Cell.h"
#include "PropertyKey.h"
#include "Value.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace Ore {
class Object : public GC::Cell {
  public:
  Object() { }
  virtual ~Object() { }

  std::map<std::string, Value> properties() const { return m_properties; }

  virtual Value get(PropertyKey key) const;
  virtual void put(PropertyKey key, Value value);
  virtual void put_native_function(PropertyKey key, std::function<Value(std::vector<Value>)>);
  virtual bool contains(PropertyKey key) const;

  virtual bool is_function() const { return false; }
  virtual bool is_native_function() const { return false; }
  virtual bool is_array() const { return false; }

  virtual void visit_graph(Visitor&) override;
  virtual char const* class_name() const override { return "Object"; }

  protected:
  std::map<std::string, Value> m_properties;
};

}
