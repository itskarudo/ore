#include "AST.h"
#include "Runtime/ArrayObject.h"
#include "Runtime/FunctionObject.h"
#include "Runtime/NativeFunction.h"
#include <fmt/core.h>

static void print_indent(int indent)
{
  for (int i = 0; i < indent * 2; i++)
    putchar(' ');
}

namespace Ore::AST {

void NumberLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << m_value << "\033[0m" << std::endl;
}

Result NumberLiteral::execute(Interpreter& interpreter)
{
  return ore_number(m_value);
}

void BooleanLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << (m_value ? "true" : "false") << "\033[0m" << std::endl;
}

Result BooleanLiteral::execute(Interpreter& interpreter)
{
  return ore_boolean(m_value);
}

void StringLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << m_value << "\033[0m" << std::endl;
}

Result StringLiteral::execute(Interpreter& interpreter)
{
  return ore_string(interpreter.heap(), m_value);
}

void NilLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[0m" << std::endl;
}

Result NilLiteral::execute(Interpreter& interpreter)
{
  return ore_nil();
}

void BlockStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  for (auto& child : children())
    child->dump_impl(indent + 1);
}

Result BlockStatement::execute(Interpreter& interpreter)
{
  return interpreter.run(*this);
}

void FunctionDeclaration::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << name().value_or("(anonymous)") << "\033[0m\n";

  for (auto& parameter : parameters()) {
    print_indent(indent + 1);
    printf("\033[34mParameter: '%s'\033[0m\n", parameter.name.c_str());
    if (parameter.default_value.has_value())
      parameter.default_value.value()->dump_impl(indent + 2);
  }

  body()->dump_impl(indent + 1);
}

Result FunctionDeclaration::execute(Interpreter& interpreter)
{
  bool did_find_optional = false;

  std::vector<FunctionObject::Parameter> parameter_values;

  for (auto& parameter : parameters()) {
    FunctionObject::Parameter parameter_value;
    parameter_value.name = parameter.name;

    // make sure all required parameters come before the optionals
    if (did_find_optional)
      assert(parameter.default_value.has_value());

    if (parameter.default_value.has_value()) {
      did_find_optional = true;
      auto default_value = TRY(parameter.default_value.value()->execute(interpreter));

      parameter_value.default_value = default_value;
    } else
      parameter_value.default_value = std::nullopt;

    parameter_values.push_back(parameter_value);
  }

  auto* function = interpreter.heap().allocate<FunctionObject>(name(), body(), std::move(parameter_values));

  if (name().has_value())
    interpreter.set_variable(name().value(), function);

  return Value(function);
}

void CallExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p}\033[0m\n", class_name(), this);

  m_callee->dump_impl(indent + 1);

  for (auto& argument : m_arguments)
    argument->dump_impl(indent + 1);
}

Result CallExpression::execute(Interpreter& interpreter)
{
  Value value;
  if (m_callee->is_identifier()) {
    value = TRY(interpreter.get_variable(static_cast<Identifier&>(*m_callee).name()));
  } else if (m_callee->is_member_expression()) {
    value = TRY(static_cast<MemberExpression&>(*m_callee).execute(interpreter));
  } else
    __builtin_unreachable();

  if (!value.is_object())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "cannot call non-function value");

  auto* callee = value.as_object();

  if (callee->is_function()) {
    auto& function = static_cast<FunctionObject&>(*callee);

    if (m_arguments.size() > function.parameters().size())
      return interpreter.throw_exception(ExceptionObject::type_exception(), fmt::format("function only takes {} parameters", function.parameters().size()));

    std::map<std::string, Value> passed_arguments;

    for (size_t i = 0; i < function.parameters().size(); ++i) {
      if (i < m_arguments.size()) {
        auto argument_value = TRY(m_arguments[i]->execute(interpreter));
        passed_arguments[function.parameters()[i].name] = argument_value;
      } else {
        assert(function.parameters()[i].default_value.has_value());
        passed_arguments[function.parameters()[i].name] = function.parameters()[i].default_value.value();
      }
    }

    return interpreter.run(*function.body(), passed_arguments);

  } else if (callee->is_native_function()) {
    auto& function = static_cast<NativeFunction&>(*callee);

    std::vector<Value> passed_arguments;
    for (auto& argument : m_arguments) {
      auto argument_value = TRY(argument->execute(interpreter));
      passed_arguments.push_back(argument_value);
    }

    return TRY(function.native_function()(interpreter, passed_arguments));
  }

  return interpreter.throw_exception(ExceptionObject::type_exception(), "cannot call non-function value");
}

void ReturnStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  argument().dump_impl(indent + 1);
}

Result ReturnStatement::execute(Interpreter& interpreter)
{
  auto argument_value = TRY(argument().execute(interpreter));
  return { Result::Type::Return, argument_value };
}

void BreakStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
}

Result BreakStatement::execute(Interpreter& interpreter)
{
  return { Result::Type::Break, {} };
}

void ContinueStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
}

Result ContinueStatement::execute(Interpreter& interpreter)
{
  return { Result::Type::Continue, {} };
}

void IfStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  test().dump_impl(indent + 1);

  consequent().dump_impl(indent + 1);

  if (m_alternate.has_value()) {
    print_indent(indent);
    printf("\033[32mElse\033[0m\n");
    alternate().dump_impl(indent + 1);
  }
}

Result IfStatement::execute(Interpreter& interpreter)
{
  Result return_value = ore_nil();
  auto test_value = TRY(test().execute(interpreter));
  if (test_value.to_boolean())
    return_value = consequent().execute(interpreter);
  else if (m_alternate.has_value())
    return_value = alternate().execute(interpreter);

  return return_value;
}

void ForStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);

  if (m_init.has_value())
    m_init.value()->dump_impl(indent + 1);
  if (m_test.has_value())
    m_test.value()->dump_impl(indent + 1);
  if (m_update.has_value())
    m_update.value()->dump_impl(indent + 1);

  m_body->dump_impl(indent + 1);
}

Result ForStatement::execute(Interpreter& interpreter)
{
  Result return_value = ore_nil();

  if (m_init.has_value()) {
    TRY(m_init.value()->execute(interpreter));
  }

  bool test_value = true;
  if (m_test.has_value()) {
    test_value = TRY(m_test.value()->execute(interpreter)).to_boolean();
  }

  while (test_value) {
    return_value = m_body->execute(interpreter);

    if (return_value.is_exception())
      return return_value;

    if (return_value.type() == Result::Type::Break)
      break;

    if (m_update.has_value()) {
      TRY(m_update.value()->execute(interpreter));
    }

    test_value = TRY(m_test.value()->execute(interpreter)).to_boolean();
  }

  return return_value;
}

void WhileStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  test().dump_impl(indent + 1);
  body().dump_impl(indent + 1);
}

Result WhileStatement::execute(Interpreter& interpreter)
{

  Result return_value = ore_nil();

  while (TRY(test().execute(interpreter)).to_boolean()) {
    return_value = body().execute(interpreter);

    if (return_value.is_exception())
      return return_value;

    if (return_value.type() == Result::Type::Break)
      break;
  }

  return return_value;
}

void DoWhileStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  test().dump_impl(indent + 1);
  body().dump_impl(indent + 1);
}

Result DoWhileStatement::execute(Interpreter& interpreter)
{

  Result return_value = ore_nil();

  do {
    return_value = body().execute(interpreter);

    if (return_value.is_exception())
      return return_value;

    if (return_value.type() == Result::Type::Break)
      break;
  } while (TRY(test().execute(interpreter)).to_boolean());

  return return_value;
}

void Identifier::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m%s \033[0m\n", class_name(), this, name().c_str());
}

Result Identifier::execute(Interpreter& interpreter)
{
  return TRY(interpreter.get_variable(name()));
}

void AssignmentExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_lhs->dump_impl(indent + 1);
  m_rhs->dump_impl(indent + 1);
}

Result AssignmentExpression::execute(Interpreter& interpreter)
{
  auto value = TRY(m_rhs->execute(interpreter));
  Value prev_value;
  PropertyKey key;
  Object* object = nullptr;

  if (m_lhs->is_identifier()) {
    auto id = static_cast<Identifier&>(*m_lhs);
    key = id.name();

    if (m_op != Op::Assignment) {
      prev_value = TRY(id.execute(interpreter));
    }

  } else if (m_lhs->is_member_expression()) {
    auto& member_expression = static_cast<MemberExpression&>(*m_lhs);

    auto object_value = TRY(member_expression.object().execute(interpreter));
    object = object_value.to_object(interpreter.heap());

    if (member_expression.is_computed()) {
      auto computed_value = TRY(member_expression.property().execute(interpreter));
      if (computed_value.is_number())
        key = computed_value.as_number();
      else if (computed_value.is_string())
        key = computed_value.as_string()->string();
      else
        return interpreter.throw_exception(ExceptionObject::type_exception(), "key must be a string or a number");
    } else {
      assert(member_expression.property().is_identifier());
      auto& id = static_cast<Identifier&>(member_expression.property());
      key = id.name();
    }

    if (m_op != Op::Assignment)
      prev_value = TRY(object->get(key));

  } else {
    __builtin_unreachable();
  }

  switch (m_op) {
  case Op::AddAssignment:
    value = TRY(Value::add(interpreter, prev_value, value));
    break;
  case Op::SubAssignment:
    value = TRY(Value::subtract(interpreter, prev_value, value));
    break;
  case Op::MultAssignment:
    value = TRY(Value::multiply(interpreter, prev_value, value));
    break;
  case Op::DivAssignment:
    value = TRY(Value::divide(interpreter, prev_value, value));
    break;
  case Op::ShiftLeftAssignment:
    value = TRY(Value::shift_left(interpreter, prev_value, value));
    break;
  case Op::ShiftRightAssignment:
    value = TRY(Value::shift_right(interpreter, prev_value, value));
    break;
  case Op::ConcatAssignment:
    value = TRY(Value::string_concat(interpreter, prev_value, value));
    break;
  default:
    break;
  }

  if (m_lhs->is_identifier())
    interpreter.set_variable(key.string(), value);
  else if (m_lhs->is_member_expression())
    object->put(key, value);
  else
    __builtin_unreachable();

  return value;
}

void GlobalStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_assignment->dump_impl(indent + 1);
}

Result GlobalStatement::execute(Interpreter& interpreter)
{
  assert(m_assignment->lhs().is_identifier());
  auto& id = static_cast<Identifier&>(m_assignment->lhs());
  auto value = TRY(m_assignment->rhs().execute(interpreter));
  interpreter.global_object()->put(PropertyKey(id.name()), value);
  return value;
}

void UnaryExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m", class_name(), this);
  switch (m_op) {
  case Op::Not:
    printf("not");
    break;
  case Op::Length:
    putchar('#');
    break;
  }
  printf("\033[0m\n");
  m_operand->dump_impl(indent + 1);
}

Result UnaryExpression::execute(Interpreter& interpreter)
{
  auto value = TRY(m_operand->execute(interpreter));
  switch (m_op) {
  case Op::Not:
    return TRY(Value::logical_not(interpreter, value));
  case Op::Length:
    return TRY(Value::length(interpreter, value));
  default:
    __builtin_unreachable();
  }
}

void BinaryExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m", class_name(), this);
  switch (m_op) {
#define __ENUM_BI_OP(op, sym) \
  case Op::op:                \
    printf(sym);              \
    break;
    ENUMERATE_BINARY_OPS
#undef __ENUM_BI_OP
  }
  printf("\033[0m\n");
  m_lhs->dump_impl(indent + 1);
  m_rhs->dump_impl(indent + 1);
}

Result BinaryExpression::execute(Interpreter& interpreter)
{

  auto lhs_value = TRY(m_lhs->execute(interpreter));

  if (m_op == Op::And && !lhs_value.to_boolean())
    return ore_boolean(false);
  else if (m_op == Op::Or && lhs_value.to_boolean())
    return ore_boolean(true);

  auto rhs_value = TRY(m_rhs->execute(interpreter));

  switch (m_op) {
  case Op::Add: {
    return TRY(Value::add(interpreter, lhs_value, rhs_value));
  }
  case Op::Sub: {
    return TRY(Value::subtract(interpreter, lhs_value, rhs_value));
  }
  case Op::Mult: {
    return TRY(Value::multiply(interpreter, lhs_value, rhs_value));
  }
  case Op::Div: {
    return TRY(Value::divide(interpreter, lhs_value, rhs_value));
  }
  case Op::Equals: {
    return TRY(Value::equals(interpreter, lhs_value, rhs_value));
  }
  case Op::NotEquals: {
    return TRY(Value::not_equals(interpreter, lhs_value, rhs_value));
  }
  case Op::GreaterThan: {
    return TRY(Value::greater_than(interpreter, lhs_value, rhs_value));
  }
  case Op::LessThan: {
    return TRY(Value::less_than(interpreter, lhs_value, rhs_value));
  }
  case Op::GreaterThanOrEquals: {
    return TRY(Value::greater_than_or_equals(interpreter, lhs_value, rhs_value));
  }
  case Op::LessThanOrEquals: {
    return TRY(Value::less_than_or_equals(interpreter, lhs_value, rhs_value));
  }
  case Op::ShiftLeft: {
    return TRY(Value::shift_left(interpreter, lhs_value, rhs_value));
  }
  case Op::ShiftRight: {
    return TRY(Value::shift_right(interpreter, lhs_value, rhs_value));
  }
  case Op::StringConcat: {
    return TRY(Value::string_concat(interpreter, lhs_value, rhs_value));
  }
  case Op::Xor: {
    return TRY(Value::logical_xor(interpreter, lhs_value, rhs_value));
  }
  case Op::And: {
    return TRY(Value::logical_and(interpreter, lhs_value, rhs_value));
  }
  case Op::Or: {
    return TRY(Value::logical_or(interpreter, lhs_value, rhs_value));
  }
  default:
    return ore_nil();
  }
}

void MemberExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p} \033[34m{computed: %s}\033[0m\n", class_name(), this, is_computed() ? "true" : "false");
  object().dump_impl(indent + 1);
  property().dump_impl(indent + 1);
}

Result MemberExpression::execute(Interpreter& interpreter)
{
  auto value = TRY(object().execute(interpreter));

  if (value.is_nil())
    return interpreter.throw_exception(ExceptionObject::type_exception(), "cannot access properties of nil");

  auto obj = value.to_object(interpreter.heap());

  PropertyKey key;

  if (is_computed()) {
    auto computed_value = TRY(property().execute(interpreter));
    if (computed_value.is_number())
      key = PropertyKey(computed_value.as_number());
    else if (computed_value.is_string())
      key = PropertyKey(computed_value.as_string()->string());
    else
      return interpreter.throw_exception(ExceptionObject::type_exception(), "key must be a string or a number");

  } else {
    assert(property().is_identifier());
    auto id = static_cast<Identifier&>(property());
    key = PropertyKey(id.name());
  }

  if (obj->contains(key))
    return TRY(obj->get(key));

  return ore_nil();
}

void ObjectExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  for (auto& [key, value] : m_properties) {
    print_indent(indent + 1);
    printf("\033[34mKey: %s\033[0m\n", key.c_str());
    value->dump_impl(indent + 2);
  }
}

Result ObjectExpression::execute(Interpreter& interpreter)
{

  auto* object = interpreter.heap().allocate<Object>();

  for (auto& [key, value] : m_properties) {
    auto v = TRY(value->execute(interpreter));
    object->put(key, v);
  }

  return Value(object);
}

void ArrayExpression::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[35m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  for (auto& element : m_elements)
    element->dump_impl(indent + 1);
}

Result ArrayExpression::execute(Interpreter& interpreter)
{
  std::vector<Value> values;
  for (auto& element : m_elements) {
    auto value = TRY(element->execute(interpreter));
    values.push_back(value);
  }

  return Value(interpreter.heap().allocate<ArrayObject>(values));
}

void ExportStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_argument->dump_impl(indent + 1);
}

Result ExportStatement::execute(Interpreter& interpreter)
{
  // FIXME: Actually implement exports.
  TRY(m_argument->execute(interpreter));
  return ore_nil();
}

void CatchClause::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[34m %s \033[0m\n", class_name(), this, m_param.c_str());
  m_body->dump_impl(indent + 1);
}

Result CatchClause::execute(Interpreter& interpreter)
{
  // CatchClause execution is handled by TryStatement
  assert(false);
}

void TryStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_block->dump_impl(indent + 1);
  m_handler->dump_impl(indent + 1);

  if (m_finalizer.has_value()) {
    printf("\033[34m Finalizer\033[0m\n");
    m_finalizer.value()->dump_impl(indent + 1);
  }
}

Result TryStatement::execute(Interpreter& interpreter)
{
  Result return_value = ore_nil();

  return_value = interpreter.run(*m_block);

  if (return_value.is_exception()) {

    std::map<std::string, Value> arguments;
    arguments[m_handler->param()] = return_value.value();
    return_value = TRY(interpreter.run(m_handler->body(), std::move(arguments)));
  }

  if (m_finalizer.has_value())
    return_value = m_finalizer.value()->execute(interpreter);

  return return_value;
}

}
