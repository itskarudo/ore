#pragma once

#include "Object.h"
#include "PrimitiveString.h"

namespace Ore {
class StringObject final : public Object {
  public:
  explicit StringObject(PrimitiveString* value)
      : m_value(value)
  {
  }

  virtual ~StringObject() { }
  virtual void visit_graph(Visitor&) override;

  PrimitiveString* const& value() const { return m_value; }
  virtual char const* class_name() const override { return "StringObject"; }

  private:
  PrimitiveString* m_value;
};

}
