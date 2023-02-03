#pragma once
#include "Object.h"
#include "PrimitiveString.h"

namespace Ore {
class ExceptionObject : public Object {
  public:
  ExceptionObject(std::string const& message)
      : m_message(std::move(message))
  {
    put(PropertyKey("message"), ore_string(heap(), message));
  }
  virtual ~ExceptionObject() { }

  virtual char const* class_name() const override { return "ExceptionObject"; }
  std::string message() const { return m_message; }

  private:
  std::string m_message;
};

}
