#pragma once

#include "Object.h"

namespace Ore {
class StringObject final : public Object {
  public:
  explicit StringObject(std::string const& value)
      : m_value(value)
  {
  }

  virtual ~StringObject() { }

  std::string const& value() const { return m_value; }
  virtual char const* class_name() const override { return "StringObject"; }

  private:
  std::string m_value;
};

}
