#include "AST.h"
#include "Runtime/FunctionObject.h"
#include <iostream>

#include <assert.h>

static void print_indent(int indent)
{
  for (int i = 0; i < indent; i++)
    putchar(' ');
}

namespace Ore::AST {

void Literal::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << value() << std::endl;
}

Value Literal::execute(Interpreter& interpreter)
{
  return value();
}

void ScopeNode::dump_impl(int indent) const
{
  print_indent(indent);
  printf("%s\n", class_name());
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
  printf("%s<%s>\n", class_name(), name().c_str());
  print_indent(indent);
  printf(">\n");
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
  printf("%s<%s>\n", class_name(), name().c_str());
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
  printf("%s<\n", class_name());
  argument().dump_impl(indent + 1);
  print_indent(indent);
  printf(">\n");
}

Value ReturnStatement::execute(Interpreter& interpreter)
{
  return argument().execute(interpreter);
}

void IfStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("If<\n");
  test().dump_impl(indent + 1);
  print_indent(indent);
  printf(">\n");

  consequent().dump_impl(indent + 1);

  print_indent(indent);
  printf("Else\n");

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
  printf("While<\n");
  test().dump_impl(indent + 1);
  printf(">\n");
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
  printf("%s<%s>\n", class_name(), name().c_str());
}

Value Identifier::execute(Interpreter& interpreter)
{
  return interpreter.get_variable(name());
}

void VariableDeclaration::dump_impl(int indent) const
{
  print_indent(indent);
  printf("%s<\n", class_name());
  id().dump_impl(indent + 1);
  print_indent(indent);
  printf(" = \n");
  value().dump_impl(indent + 1);
  print_indent(indent);
  printf(">\n");
}

Value VariableDeclaration::execute(Interpreter& interpreter)
{
  interpreter.set_variable(id().name(), value().execute(interpreter));
  return Value();
}

}
