#pragma once

#include "Object.h"
#include "PrimitiveString.h"

namespace Ore {
class StringObject final : public Object {
  public:
  explicit StringObject(PrimitiveString*);

  virtual ~StringObject() { }
  virtual void visit_graph(Visitor&) override;

  PrimitiveString* const& value() const { return m_value; }
  virtual char const* class_name() const override { return "StringObject"; }
  virtual bool to_boolean() const override { return !m_value->string().empty(); }

  private:
  PrimitiveString* m_value;
};

}
