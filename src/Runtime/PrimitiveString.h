#pragma once
#include "../GC/Cell.h"

namespace Ore {
class PrimitiveString : public GC::Cell {
  public:
  PrimitiveString(std::string const& value)
      : m_value(value)
  {
  }

  std::string value() const { return m_value; }
  virtual char const* class_name() const override { return "PrimitiveString"; }

  private:
  std::string m_value;
};
}
