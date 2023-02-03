#pragma once

#include "Object.h"

namespace Ore {
class NumberObject final : public Object {
  public:
  explicit NumberObject(double value)
      : m_value(value)
  {
  }

  explicit NumberObject(int value)
      : m_value(value)
  {
  }

  virtual ~NumberObject() { }

  double value() const { return m_value; }

  virtual char const* class_name() const override { return "NumberObject"; }
  virtual bool to_boolean() const override { return m_value != 0; }

  private:
  double m_value;
};

}
