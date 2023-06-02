#pragma once

#include <optional>
#include <vector>

#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Result.h"
#include "Runtime/Value.h"
#include "SourceRange.h"

#define ENUMERATE_BINARY_OPS              \
  __ENUM_BI_OP(Add, "+")                  \
  __ENUM_BI_OP(Sub, "-")                  \
  __ENUM_BI_OP(Mult, "*")                 \
  __ENUM_BI_OP(Div, "/")                  \
  __ENUM_BI_OP(Pow, "**")                 \
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
  __ENUM_BI_OP(StringConcat, "..")        \
  __ENUM_BI_OP(Modulo, "%%")

#define ENUMERATE_ASSIGNMENT_OPS                    \
  __ENUM_ASSIGNMENT_OP(Assignment, "=")             \
  __ENUM_ASSIGNMENT_OP(AddAssignment, "+=")         \
  __ENUM_ASSIGNMENT_OP(SubAssignment, "-=")         \
  __ENUM_ASSIGNMENT_OP(MultAssignment, "*=")        \
  __ENUM_ASSIGNMENT_OP(DivAssignment, "/=")         \
  __ENUM_ASSIGNMENT_OP(ShiftLeftAssignment, "<<=")  \
  __ENUM_ASSIGNMENT_OP(ShiftRightAssignment, ">>=") \
  __ENUM_ASSIGNMENT_OP(ConcatAssignment, "..=")

namespace Ore::AST {

class ASTNode {
  public:
  virtual ~ASTNode() = default;
  void dump() const { dump_impl(0); }
  virtual char const* class_name() const = 0;
  virtual void dump_impl(int indent) const = 0;
  virtual Result execute(Interpreter&) = 0;

  SourceRange const& source_range() const { return m_source_range; }

  virtual bool is_identifier() const { return false; }
  virtual bool is_member_expression() const { return false; }
  virtual bool is_function_declaration() const { return false; }
  virtual bool is_program() const { return false; }

  protected:
  explicit ASTNode(SourceRange source_range)
      : m_source_range(source_range)
  {
  }

  private:
  SourceRange m_source_range;
};

class Statement : public ASTNode {
  protected:
  explicit Statement(SourceRange source_range)
      : ASTNode(source_range)
  {
  }
};

class Expression : public Statement {
  protected:
  explicit Expression(SourceRange source_range)
      : Statement(source_range)
  {
  }
};

class BlockStatement : public Statement {
  public:
  explicit BlockStatement(SourceRange source_range)
      : Statement(source_range)
  {
  }
  virtual char const* class_name() const override { return "BlockStatement"; };
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  std::vector<std::unique_ptr<ASTNode>> const& children() const { return m_children; };

  template<typename T, typename... Args>
  void append(Args&&... args)
  {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    m_children.push_back(std::move(child));
  }

  void append(std::unique_ptr<ASTNode> child)
  {
    m_children.push_back(std::move(child));
  }

  private:
  std::vector<std::unique_ptr<ASTNode>> m_children;
};

class Program : public BlockStatement {
  public:
  explicit Program(SourceRange source_range)
      : BlockStatement(source_range)
  {
  }
  virtual char const* class_name() const override { return "Program"; };
  virtual bool is_program() const override { return true; }
};

class Literal : public Expression {
  protected:
  explicit Literal(SourceRange source_range)
      : Expression(source_range)
  {
  }
};

class NumberLiteral : public Literal {
  public:
  NumberLiteral(SourceRange source_range, double value)
      : Literal(source_range)
      , m_value(value)
  {
  }

  virtual char const* class_name() const override { return "NumberLiteral"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  double m_value;
};

class BooleanLiteral : public Literal {
  public:
  BooleanLiteral(SourceRange source_range, bool value)
      : Literal(source_range)
      , m_value(value)
  {
  }

  virtual char const* class_name() const override { return "BooleanLiteral"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  bool m_value;
};

class StringLiteral : public Literal {
  public:
  StringLiteral(SourceRange source_range, std::string const& value)
      : Literal(source_range)
      , m_value(std::move(value))
  {
  }

  virtual char const* class_name() const override { return "StringLiteral"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::string const m_value;
};

class NilLiteral : public Literal {
  public:
  NilLiteral(SourceRange source_range)
      : Literal(source_range)
  {
  }

  virtual char const* class_name() const override { return "NilLiteral"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;
};

class FunctionDeclaration : public Expression {
  public:
  struct Parameter {
    std::string name;
    std::optional<std::unique_ptr<Expression>> default_value;
  };

  FunctionDeclaration(SourceRange source_range, std::optional<std::string> name, std::shared_ptr<BlockStatement> body, std::vector<Parameter> parameters = {})
      : Expression(source_range)
      , m_name(std::move(name))
      , m_body(body)
      , m_parameters(std::move(parameters))
  {
  }

  std::optional<std::string>& name() { return m_name; }
  std::optional<std::string> const& name() const { return m_name; }
  std::shared_ptr<BlockStatement> body() const { return m_body; }
  std::vector<Parameter> const& parameters() const { return m_parameters; }

  virtual char const* class_name() const override { return "FunctionDeclaration"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  virtual bool is_function_declaration() const override { return true; }

  private:
  std::optional<std::string> m_name;
  std::shared_ptr<BlockStatement> m_body;
  std::vector<Parameter> m_parameters;
};

class CallExpression : public Expression {
  public:
  CallExpression(SourceRange source_range, std::unique_ptr<ASTNode> callee, std::vector<std::unique_ptr<Expression>> arguments = {})
      : Expression(source_range)
      , m_callee(std::move(callee))
      , m_arguments(std::move(arguments))
  {
  }

  virtual char const* class_name() const override { return "CallExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<ASTNode> m_callee;
  std::vector<std::unique_ptr<Expression>> const m_arguments;
};

class ReturnStatement : public Statement {
  public:
  ReturnStatement(SourceRange source_range, std::optional<std::unique_ptr<Expression>> argument)
      : Statement(source_range)
      , m_argument(std::move(argument))
  {
  }

  virtual char const* class_name() const override { return "ReturnStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::optional<std::unique_ptr<Expression>> m_argument;
};

class BreakStatement : public Statement {
  public:
  BreakStatement(SourceRange source_range)
      : Statement(source_range)
  {
  }

  virtual char const* class_name() const override { return "BreakStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;
};

class ContinueStatement : public Statement {
  public:
  ContinueStatement(SourceRange source_range)
      : Statement(source_range)
  {
  }

  virtual char const* class_name() const override { return "ContinueStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;
};

class IfStatement : public Statement {
  public:
  IfStatement(SourceRange source_range, std::unique_ptr<Expression> test, std::unique_ptr<Statement> consequent, std::optional<std::unique_ptr<Statement>> alternate)
      : Statement(source_range)
      , m_test(std::move(test))
      , m_consequent(std::move(consequent))
      , m_alternate(std::move(alternate))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& consequent() const { return *m_consequent; }
  Statement& alternate() const { return *m_alternate.value(); }

  virtual char const* class_name() const override { return "IfStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_consequent;
  std::optional<std::unique_ptr<Statement>> m_alternate;
};

class ForStatement : public Statement {
  public:
  ForStatement(SourceRange source_range, std::optional<std::unique_ptr<Expression>> init,
      std::optional<std::unique_ptr<Expression>> test,
      std::optional<std::unique_ptr<Expression>> update,
      std::unique_ptr<Statement> body)
      : Statement(source_range)
      , m_init(std::move(init))
      , m_test(std::move(test))
      , m_update(std::move(update))
      , m_body(std::move(body))
  {
  }

  virtual char const* class_name() const override { return "ForStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::optional<std::unique_ptr<Expression>> m_init, m_test, m_update;
  std::unique_ptr<Statement> m_body;
};

class WhileStatement : public Statement {
  public:
  WhileStatement(SourceRange source_range, std::unique_ptr<Expression> test, std::unique_ptr<Statement> body)
      : Statement(source_range)
      , m_test(std::move(test))
      , m_body(std::move(body))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& body() const { return *m_body; }

  virtual char const* class_name() const override { return "WhileStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_body;
};

class DoWhileStatement : public Statement {
  public:
  DoWhileStatement(SourceRange source_range, std::unique_ptr<Expression> test, std::unique_ptr<Statement> body)
      : Statement(source_range)
      , m_test(std::move(test))
      , m_body(std::move(body))
  {
  }

  Expression& test() const { return *m_test; }
  Statement& body() const { return *m_body; }

  virtual char const* class_name() const override { return "DoWhileStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_test;
  std::unique_ptr<Statement> m_body;
};

class Identifier : public Expression {
  public:
  Identifier(SourceRange source_range, std::string const& name)
      : Expression(source_range)
      , m_name(std::move(name))
  {
  }

  virtual char const* class_name() const override { return "Identifier"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  virtual bool is_identifier() const override { return true; }

  std::string const& name() const { return m_name; }

  private:
  std::string m_name;
};

class AssignmentExpression : public Expression {
  public:
  enum class Op {
#define __ENUM_ASSIGNMENT_OP(op, sym) op,
    ENUMERATE_ASSIGNMENT_OPS
#undef __ENUM_ASSIGNMENT_OP
  };

  AssignmentExpression(SourceRange source_range, std::unique_ptr<ASTNode> lhs, Op op, std::unique_ptr<Expression> rhs)
      : Expression(source_range)
      , m_lhs(std::move(lhs))
      , m_op(op)
      , m_rhs(std::move(rhs))
  {
  }

  virtual char const* class_name() const override { return "AssignmentExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  ASTNode& lhs() { return *m_lhs; }
  Expression& rhs() { return *m_rhs; }

  private:
  std::unique_ptr<ASTNode> m_lhs;
  std::unique_ptr<Expression> m_rhs;
  Op m_op;
};

class GlobalStatement : public Statement {
  public:
  GlobalStatement(SourceRange source_range, std::unique_ptr<AssignmentExpression> assignment)
      : Statement(source_range)
      , m_assignment(std::move(assignment))
  {
  }

  virtual char const* class_name() const override { return "GlobalStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<AssignmentExpression> m_assignment;
};

class UnaryExpression : public Expression {
  public:
  enum class Op {
    Not,
    Negate,
    Length,
  };

  UnaryExpression(SourceRange source_range, Op op, std::unique_ptr<Expression> operand)
      : Expression(source_range)
      , m_op(op)
      , m_operand(std::move(operand))
  {
  }

  virtual char const* class_name() const override { return "UnaryExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

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

  BinaryExpression(SourceRange source_range, std::unique_ptr<Expression> lhs, Op op, std::unique_ptr<Expression> rhs)
      : Expression(source_range)
      , m_lhs(std::move(lhs))
      , m_op(op)
      , m_rhs(std::move(rhs))
  {
  }

  virtual char const* class_name() const override { return "BinaryExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_lhs, m_rhs;
  Op m_op;
};

class MemberExpression : public Expression {
  public:
  explicit MemberExpression(SourceRange source_range, std::unique_ptr<Expression> object, std::unique_ptr<Expression> property, bool computed = false)
      : Expression(source_range)
      , m_object(std::move(object))
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
  virtual Result execute(Interpreter&) override;

  virtual bool is_member_expression() const override { return true; }

  private:
  std::unique_ptr<Expression> m_object;
  std::unique_ptr<Expression> m_property;
  bool m_computed { false };
};

class ObjectExpression : public Expression {
  public:
  ObjectExpression(SourceRange source_range, std::map<std::string, std::unique_ptr<Expression>> properties = {})
      : Expression(source_range)
      , m_properties(std::move(properties))
  {
  }

  virtual char const* class_name() const override { return "ObjectExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::map<std::string, std::unique_ptr<Expression>> m_properties;
};

class ArrayExpression : public Expression {
  public:
  ArrayExpression(SourceRange source_range, std::vector<std::unique_ptr<Expression>> elements = {})
      : Expression(source_range)
      , m_elements(std::move(elements))
  {
  }

  virtual char const* class_name() const override { return "ArrayExpression"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::vector<std::unique_ptr<Expression>> m_elements;
};

class ExportStatement : public Statement {
  public:
  ExportStatement(SourceRange source_range, std::unique_ptr<Expression> argument)
      : Statement(source_range)
      , m_argument(std::move(argument))
  {
  }

  virtual char const* class_name() const override { return "ExportStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<Expression> m_argument;
};

class CatchClause : public ASTNode {
  public:
  CatchClause(SourceRange source_range, std::string param, std::unique_ptr<BlockStatement> body)
      : ASTNode(source_range)
      , m_param(std::move(param))
      , m_body(std::move(body))
  {
  }

  virtual char const* class_name() const override { return "CatchClause"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  std::string const& param() const { return m_param; }
  BlockStatement& body() const { return *m_body; }

  private:
  std::string m_param;
  std::unique_ptr<BlockStatement> m_body;
};

class TryStatement : public Statement {
  public:
  TryStatement(SourceRange source_range, std::unique_ptr<BlockStatement> block, std::unique_ptr<CatchClause> handler, std::optional<std::unique_ptr<BlockStatement>> finalizer)
      : Statement(source_range)
      , m_block(std::move(block))
      , m_handler(std::move(handler))
      , m_finalizer(std::move(finalizer))
  {
  }

  virtual char const* class_name() const override { return "TryStatement"; }
  virtual void dump_impl(int indent) const override;
  virtual Result execute(Interpreter&) override;

  private:
  std::unique_ptr<BlockStatement> m_block;
  std::unique_ptr<CatchClause> m_handler;
  std::optional<std::unique_ptr<BlockStatement>> m_finalizer;
};

}
