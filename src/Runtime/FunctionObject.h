#pragma once

#include "../Forward.h"
#include "Object.h"
#include <optional>

namespace Ore {
class FunctionObject final : public Object {
  public:
  FunctionObject(std::optional<std::string> name, std::shared_ptr<AST::ScopeNode> body)
      : m_name(name)
      , m_body(body)
  {
  }

  virtual ~FunctionObject() { }

  std::optional<std::string> name() const { return m_name; }
  std::shared_ptr<AST::ScopeNode> body() const { return m_body; }

  virtual bool is_function() const override { return true; }
  virtual char const* class_name() const override { return "Function"; }

  private:
  std::optional<std::string> m_name;
  std::shared_ptr<AST::ScopeNode> m_body;
};
}
