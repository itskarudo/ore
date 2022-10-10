#include "AST.h"
#include "Runtime/FunctionObject.h"
#include <iostream>

#include <assert.h>
#include <math.h>

static void print_indent(int indent)
{
  for (int i = 0; i < indent; i++)
    putchar(' ');
}

namespace Ore::AST {

void Literal::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << value() << "\033[0m" << std::endl;
}

Value Literal::execute(Interpreter& interpreter)
{
  return value();
}

void ScopeNode::dump_impl(int indent) const
{
  for (auto& child : children())
    child->dump_impl(indent);
}

void Program::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  for (auto& child : children())
    child->dump_impl(indent + 1);
}

Value ScopeNode::execute(Interpreter& interpreter)
{
  return interpreter.run(*this);
}

void FunctionDeclaration::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p} \033[34m%s\033[0m \n", class_name(), this, name().c_str());
  print_indent(indent);
  body()->dump_impl(indent + 1);
}

Value FunctionDeclaration::execute(Interpreter& interpreter)
{
  auto* function = interpreter.heap().allocate<FunctionObject>(name(), body());
  interpreter.set_variable(name(), function);

  return Value(function);
}

void CallExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m%s\033[0m\n", class_name(), this, name().c_str());
}

Value CallExpression::execute(Interpreter& interpreter)
{
  auto value = interpreter.get_variable(name());
  assert(value.is_object());

  auto* callee = value.as_object();
  assert(callee->is_function());

  auto& function = static_cast<FunctionObject&>(*callee);
  return interpreter.run(*function.body());
}

void ReturnStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  argument().dump_impl(indent + 1);
}

Value ReturnStatement::execute(Interpreter& interpreter)
{
  return argument().execute(interpreter);
}

void IfStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  test().dump_impl(indent + 1);

  consequent().dump_impl(indent + 1);

  print_indent(indent);
  printf("\033[32mElse\033[0m\n");

  alternate().dump_impl(indent + 1);
}

Value IfStatement::execute(Interpreter& interpreter)
{
  if (test().execute(interpreter).to_boolean())
    return consequent().execute(interpreter);
  else
    return alternate().execute(interpreter);
}

void WhileStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m<\n", class_name(), this);
  test().dump_impl(indent + 1);
  body().dump_impl(indent + 1);
}

Value WhileStatement::execute(Interpreter& interpreter)
{

  Value return_value;

  while (test().execute(interpreter).to_boolean())
    return_value = body().execute(interpreter);

  return return_value;
}

void Identifier::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m%s \033[0m\n", class_name(), this, name().c_str());
}

Value Identifier::execute(Interpreter& interpreter)
{
  return interpreter.get_variable(name());
}

void AssignmentExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p} \033[34m%s\033[0m\n", class_name(), this, id().name().c_str());
  value().dump_impl(indent + 1);
}

Value AssignmentExpression::execute(Interpreter& interpreter)
{
  interpreter.set_variable(id().name(), value().execute(interpreter));
  return Value();
}

void BinaryExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m", class_name(), this);
  switch (m_op) {
  case Op::Add:
    putchar('+');
    break;
  case Op::Sub:
    putchar('-');
    break;
  case Op::Mult:
    putchar('*');
    break;
  case Op::Div:
    putchar('/');
    break;
  case Op::GreaterThan:
    putchar('>');
    break;
  case Op::LessThan:
    putchar('<');
    break;
  case Op::GreaterThanOrEquals:
    printf(">=");
    break;
  case Op::LessThanOrEquals:
    printf("<=");
    break;
  case Op::StringConcat:
    printf("..");
    break;
  }
  printf("\033[0m\n");
  m_lhs->dump_impl(indent + 1);
  m_rhs->dump_impl(indent + 1);
}

Value BinaryExpression::execute(Interpreter& interpreter)
{
  auto lhs_value = m_lhs->execute(interpreter);
  auto rhs_value = m_rhs->execute(interpreter);

  switch (m_op) {
  case Op::Add:
    return add(lhs_value, rhs_value);
  case Op::Sub:
    return sub(lhs_value, rhs_value);
  case Op::Mult:
    return multiply(lhs_value, rhs_value);
  case Op::Div:
    return divide(lhs_value, rhs_value);
  case Op::GreaterThan:
    return greater_than(lhs_value, rhs_value);
  case Op::LessThan:
    return less_than(lhs_value, rhs_value);
  case Op::GreaterThanOrEquals:
    return greater_than_or_equals(lhs_value, rhs_value);
  case Op::LessThanOrEquals:
    return less_than_or_equals(lhs_value, rhs_value);
  case Op::StringConcat:
    return string_concat(lhs_value, rhs_value, interpreter.heap());
  default:
    __builtin_unreachable();
  }
}

}
