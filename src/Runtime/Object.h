#pragma once

#include "../GC/Cell.h"
#include "Value.h"
#include <map>
#include <memory>
#include <string>

namespace Ore {
class Object : public GC::Cell {
  public:
  Object() { }
  virtual ~Object() { }

  std::map<std::string, Value> properties() const { return m_properties; }

  Value get(std::string const& key) const { return m_properties.at(key); }
  void put(std::string const& key, Value value) { m_properties[key] = value; }
  bool contains(std::string const& key) const { return m_properties.count(key); }

  virtual bool is_function() const { return false; }
  virtual bool is_native_function() const { return false; }

  virtual void visit_graph(Visitor&) override;
  virtual char const* class_name() const override { return "Object"; }

  private:
  std::map<std::string, Value> m_properties;
};

}
