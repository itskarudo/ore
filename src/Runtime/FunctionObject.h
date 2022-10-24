#pragma once

#include "../Forward.h"
#include "Object.h"
#include <optional>

namespace Ore {

class FunctionObject final : public Object {
  public:
  FunctionObject(std::optional<std::string> name, std::shared_ptr<AST::BlockStatement> body, std::vector<std::string> const& parameters)
      : m_name(name)
      , m_body(body)
      , m_parameters(parameters)
  {
  }

  virtual ~FunctionObject() { }

  std::optional<std::string> name() const { return m_name; }
  std::shared_ptr<AST::BlockStatement> body() const { return m_body; }

  virtual bool is_function() const override { return true; }
  virtual char const* class_name() const override { return "Function"; }

  std::vector<std::string>& parameters() { return m_parameters; }

  private:
  std::optional<std::string> m_name;
  std::shared_ptr<AST::BlockStatement> m_body;
  std::vector<std::string> m_parameters;
};
}
