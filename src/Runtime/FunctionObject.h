#pragma once

#include "../Forward.h"
#include "Object.h"
#include <optional>

namespace Ore {

class FunctionObject final : public Object {
  public:
  struct Parameter {
    std::string name;
    std::optional<Value> default_value;
  };

  FunctionObject(std::optional<std::string> name, std::shared_ptr<AST::BlockStatement> body, std::vector<Parameter> const& parameters)
      : m_name(name)
      , m_body(body)
      , m_parameters(std::move(parameters))
  {
  }

  virtual ~FunctionObject() { }

  std::optional<std::string> name() const { return m_name; }
  std::shared_ptr<AST::BlockStatement> body() const { return m_body; }

  virtual bool is_function() const override { return true; }
  virtual char const* class_name() const override { return "Function"; }

  std::vector<Parameter>& parameters() { return m_parameters; }

  private:
  std::optional<std::string> m_name;
  std::shared_ptr<AST::BlockStatement> m_body;
  std::vector<Parameter> m_parameters;
};
}
