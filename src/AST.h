#pragma once

#include <optional>
#include <vector>

#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Value.h"

#define ENUMERATE_BINARY_OPS              \
  __ENUM_BI_OP(Add, "+")                  \
  __ENUM_BI_OP(Sub, "-")                  \
  __ENUM_BI_OP(Mult, "*")                 \
  __ENUM_BI_OP(Div, "/")                  \
  __ENUM_BI_OP(Equals, "==")              \
  __ENUM_BI_OP(NotEquals, "!=")           \
  __ENUM_BI_OP(GreaterThan, ">")          \
  __ENUM_BI_OP(LessThan, "<")             \
  __ENUM_BI_OP(GreaterThanOrEquals, ">=") \
  __ENUM_BI_OP(LessThanOrEquals, "<=")    \
  __ENUM_BI_OP(ShiftLeft, "<<")           \
  __ENUM_BI_OP(ShiftRight, ">>")          \
  __ENUM_BI_OP(And, "and")                \
  __ENUM_BI_OP(Or, "or")                  \
  __ENUM_BI_OP(Xor, "xor")                \
  __ENUM_BI_OP(StringConcat, "..")

namespace Ore::AST {

class ASTNode {
  public:
  virtual ~ASTNode() = default;
  void dump() const { dump_impl(0); }
  virtual char const* class_name() const = 0;
  virtual void dump_impl(int indent) const = 0;
  virtual Value execute(Interpreter&) = 0;

  virtual bool is_identifier() const { return false; }
  virtual bool is_member_expression() const { return false; }
};

class Statement : public ASTNode {
};

class Expression : public Statement {
};

class BlockStatement : public Statement {
  public:
  virtual char const* class_name() const override { return "BlockStatement"; };
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  std::vector<std::unique_ptr<ASTNode>> const& children() const { return m_children; };

  template<typename T, typename... Args>
  void append(Args&&... args)
  {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    m_children.push_back(std::move(child));
  }

  private:
  std::vector<std::unique_ptr<ASTNode>> m_children;
};

class Program : public BlockStatement {
  public:
  virtual char const* class_name() const override { return "Program"; };
  virtual void dump_impl(int indent) const override;
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

class FunctionDeclaration : public Expression {
  public:
  struct Parameter {
    std::string name;
    std::optional<std::unique_ptr<Expression>> default_value;
  };

  FunctionDeclaration(std::optional<std::string> name, std::shared_ptr<BlockStatement> body, std::vector<Parameter> parameters = {})
      : m_name(name)
      , m_body(body)
      , m_parameters(std::move(parameters))
  {
  }

  std::optional<std::string> name() const { return m_name; }
  std::shared_ptr<BlockStatement> body() const { return m_body; }
  std::vector<Parameter> const& parameters() const { return m_parameters; }

  virtual char const* class_name() const override { return "FunctionDeclaration"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::optional<std::string> m_name;
  std::shared_ptr<BlockStatement> m_body;
  std::vector<Parameter> m_parameters;
};

class CallExpression : public Expression {
  public:
  CallExpression(std::unique_ptr<ASTNode> callee, std::vector<std::unique_ptr<Expression>> arguments = {})
      : m_callee(std::move(callee))
      , m_arguments(std::move(arguments))
  {
  }

  virtual char const* class_name() const override { return "CallExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<ASTNode> m_callee;
  std::vector<std::unique_ptr<Expression>> const m_arguments;
};

class ReturnStatement : public Statement {
  public:
  ReturnStatement()
      : m_argument(std::make_unique<Literal>(ore_nil()))
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
  IfStatement(std::unique_ptr<Expression> test, std::unique_ptr<Statement> consequent, std::unique_ptr<Statement> alternate)
      : m_test(std::move(test))
      , m_consequent(std::move(consequent))
      , m_alternate(std::move(alternate))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& consequent() const { return *m_consequent; }
  Statement& alternate() const { return *m_alternate; }

  virtual char const* class_name() const override { return "IfStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_consequent, m_alternate;
};

class ForStatement : public Statement {
  public:
  ForStatement(std::optional<std::unique_ptr<Expression>> init,
      std::optional<std::unique_ptr<Expression>> test,
      std::optional<std::unique_ptr<Expression>> update,
      std::unique_ptr<Statement> body)
      : m_init(std::move(init))
      , m_test(std::move(test))
      , m_update(std::move(update))
      , m_body(std::move(body))
  {
  }

  virtual char const* class_name() const override { return "ForStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::optional<std::unique_ptr<Expression>> m_init, m_test, m_update;
  std::unique_ptr<Statement> m_body;
};

class WhileStatement : public Statement {
  public:
  WhileStatement(std::unique_ptr<Expression> test, std::unique_ptr<Statement> body)
      : m_test(std::move(test))
      , m_body(std::move(body))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& body() const { return *m_body; }

  virtual char const* class_name() const override { return "WhileStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_body;
};

class DoWhileStatement : public Statement {
  public:
  DoWhileStatement(std::unique_ptr<Expression> test, std::unique_ptr<Statement> body)
      : m_test(std::move(test))
      , m_body(std::move(body))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& body() const { return *m_body; }

  virtual char const* class_name() const override { return "DoWhileStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_body;
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

  virtual bool is_identifier() const override { return true; }

  std::string name() const { return m_name; }

  private:
  std::string m_name;
};

class AssignmentExpression : public Expression {
  public:
  enum class Op {
    Assignment,
    AddAssignment,
    SubAssignment,
    MultAssignment,
    DivAssignment,
    ShiftLeftAssignment,
    ShiftRightAssignment,
    ConcatAssignment
  };

  AssignmentExpression(std::unique_ptr<ASTNode> lhs, Op op, std::unique_ptr<Expression> rhs)
      : m_lhs(std::move(lhs))
      , m_op(op)
      , m_rhs(std::move(rhs))
  {
  }

  virtual char const* class_name() const override { return "AssignmentExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  ASTNode& lhs() { return *m_lhs; }
  Expression& rhs() { return *m_rhs; }

  private:
  std::unique_ptr<ASTNode> m_lhs;
  std::unique_ptr<Expression> m_rhs;
  Op m_op;
};

class GlobalStatement : public Statement {
  public:
  GlobalStatement(std::unique_ptr<AssignmentExpression> assignment)
      : m_assignment(std::move(assignment))
  {
  }

  virtual char const* class_name() const override { return "GlobalStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<AssignmentExpression> m_assignment;
};

class UnaryExpression : public Expression {
  public:
  enum class Op {
    Not,
    Length
  };

  UnaryExpression(Op op, std::unique_ptr<Expression> operand)
      : m_op(op)
      , m_operand(std::move(operand))
  {
  }

  virtual char const* class_name() const override { return "UnaryExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  Op m_op;
  std::unique_ptr<Expression> m_operand;
};

class BinaryExpression : public Expression {
  public:
  enum class Op {
#define __ENUM_BI_OP(op, sym) op,
    ENUMERATE_BINARY_OPS
#undef __ENUM_BI_OP
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

class MemberExpression : public Expression {
  public:
  explicit MemberExpression(std::unique_ptr<Expression> object, std::unique_ptr<Expression> property, bool computed = false)
      : m_object(std::move(object))
      , m_property(std::move(property))
      , m_computed(computed)
  {
  }

  Expression& object() { return *m_object; }
  Expression const& object() const { return *m_object; }
  Expression& property() { return *m_property; }
  Expression const& property() const { return *m_property; }

  bool is_computed() const { return m_computed; }

  virtual char const* class_name() const override { return "MemberExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  virtual bool is_member_expression() const override { return true; }

  private:
  std::unique_ptr<Expression> m_object;
  std::unique_ptr<Expression> m_property;
  bool m_computed { false };
};

class ObjectExpression : public Expression {
  public:
  ObjectExpression(std::map<std::string, std::unique_ptr<Expression>> properties = {})
      : m_properties(std::move(properties))
  {
  }

  virtual char const* class_name() const override { return "ObjectExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::map<std::string, std::unique_ptr<Expression>> m_properties;
};

class ArrayExpression : public Expression {
  public:
  ArrayExpression(std::vector<std::unique_ptr<Expression>> elements = {})
      : m_elements(std::move(elements))
  {
  }

  virtual char const* class_name() const override { return "ArrayExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::vector<std::unique_ptr<Expression>> m_elements;
};

class ExportStatement : public Statement {
  public:
  ExportStatement(std::unique_ptr<Expression> argument)
      : m_argument(std::move(argument))
  {
  }

  virtual char const* class_name() const override { return "ExportStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Value execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_argument;
};

}
