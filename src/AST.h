#pragma once

#include <vector>

#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Value.h"

namespace Ore::AST {

class ASTNode {
  public:
  virtual ~ASTNode() = default;
  void dump() const { dump_impl(0); }
  virtual char const* class_name() const = 0;
  virtual void dump_impl(int indent) const = 0;
  virtual Value execute(Interpreter&) = 0;
};

class ScopeNode : public ASTNode {
  public:
  virtual char const* class_name() const override { return "ScopeNode"; };
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  std::vector<std::shared_ptr<ASTNode>> children() const { return m_children; };

  template<typename T, typename... Args>
  void append(Args&&... args)
  {
    auto child = std::make_shared<T>(std::forward<Args>(args)...);
    m_children.push_back(std::move(child));
  }

  private:
  std::vector<std::shared_ptr<ASTNode>> m_children;
};

class Program : public ScopeNode {
  public:
  virtual char const* class_name() const override { return "Program"; };
  virtual void dump_impl(int indent) const override;
};

class Statement : public ASTNode {
};

class Expression : public Statement {
};

class Literal : public Expression {
  public:
  Literal(Value value)
      : m_value(value)
  {
  }

  Value value() { return m_value; }
  const Value value() const { return m_value; }

  virtual char const* class_name() const override { return "Literal"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  Value m_value;
};

class FunctionDeclaration : public Statement {
  public:
  FunctionDeclaration(std::string const& name, std::shared_ptr<ScopeNode> body)
      : m_name(name)
      , m_body(body)
  {
  }

  std::string name() const { return m_name; }
  std::shared_ptr<ScopeNode> body() const { return m_body; }

  virtual char const* class_name() const override { return "FunctionDeclaration"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::string m_name;
  std::shared_ptr<ScopeNode> m_body;
};

class CallExpression : public Expression {
  public:
  CallExpression(std::string const& name)
      : m_name(name)
  {
  }

  std::string const& name() const { return m_name; }
  virtual char const* class_name() const override { return "CallExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::string m_name;
};

class ReturnStatement : public Statement {
  public:
  ReturnStatement()
      : m_argument(std::make_unique<Literal>(Value()))
  {
  }

  ReturnStatement(std::unique_ptr<Expression> argument)
      : m_argument(std::move(argument))
  {
  }

  Expression& argument() const { return *m_argument; }
  virtual char const* class_name() const override { return "ReturnStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_argument;
};

class IfStatement : public Statement {
  public:
  IfStatement(std::unique_ptr<Expression> test, std::unique_ptr<ScopeNode> consequent, std::unique_ptr<ScopeNode> alternate)
      : m_test(std::move(test))
      , m_consequent(std::move(consequent))
      , m_alternate(std::move(alternate))
  {
  }

  Expression& test() const { return *m_test; }
  ScopeNode& consequent() const { return *m_consequent; }
  ScopeNode& alternate() const { return *m_alternate; }

  virtual char const* class_name() const override { return "IfStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<ScopeNode> m_consequent, m_alternate;
};

class WhileStatement : public Statement {
  public:
  WhileStatement(std::unique_ptr<Expression> test, std::unique_ptr<ScopeNode> body)
      : m_test(std::move(test))
      , m_body(std::move(body))
  {
  }

  Expression& test() const { return *m_test; }
  ScopeNode& body() const { return *m_body; }

  virtual char const* class_name() const override { return "WhileStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<ScopeNode> m_body;
};

class Identifier : public Expression {
  public:
  Identifier(std::string const& name)
      : m_name(name)
  {
  }

  virtual char const* class_name() const override { return "Identifier"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  std::string name() const { return m_name; }

  private:
  std::string m_name;
};

class VariableDeclaration : public Statement {
  public:
  VariableDeclaration(std::unique_ptr<Identifier> id, std::unique_ptr<Expression> value)
      : m_id(std::move(id))
      , m_value(std::move(value))
  {
  }

  virtual char const* class_name() const override { return "VariableDeclaration"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  Identifier const& id() const { return *m_id; }
  Expression& value() { return *m_value; }
  Expression const& value() const { return *m_value; }

  private:
  std::unique_ptr<Identifier> m_id;
  std::unique_ptr<Expression> m_value;
};

class BinaryExpression : public Expression {
  public:
  enum class Op {
    Add,
    Sub,
    Mult,
    Div,
  };

  BinaryExpression(std::unique_ptr<Expression> lhs, Op op, std::unique_ptr<Expression> rhs)
      : m_lhs(std::move(lhs))
      , m_op(op)
      , m_rhs(std::move(rhs))
  {
  }

  virtual char const* class_name() const override { return "BinaryExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_lhs, m_rhs;
  Op m_op;
};

}
