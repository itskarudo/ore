#pragma once
#include "../GC/Cell.h"

namespace Ore {
class PrimitiveString : public GC::Cell {
  public:
  PrimitiveString(std::string const& string)
      : m_string(string)
  {
  }

  std::string const& string() const { return m_string; }
  virtual char const* class_name() const override { return "PrimitiveString"; }

  private:
  std::string m_string;
};
}
