#pragma once

#include "Object.h"

namespace Ore {
class BooleanObject final : public Object {
  public:
  BooleanObject(bool value)
      : m_value(value)
  {
  }

  virtual ~BooleanObject() { }

  bool value() const { return m_value; }
  virtual char const* class_name() const override { return "BooleanObject"; }

  private:
  bool m_value;
};

}
