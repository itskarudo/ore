#include "AST.h"
#include "Runtime/ArrayObject.h"
#include "Runtime/FunctionObject.h"
#include "Runtime/NativeFunction.h"

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

Value NumberLiteral::execute(Interpreter& interpreter)
{
  return ore_number(m_value);
}

void BooleanLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << (m_value ? "true" : "false") << "\033[0m" << std::endl;
}

Value BooleanLiteral::execute(Interpreter& interpreter)
{
  return ore_boolean(m_value);
}

void StringLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[34m" << m_value << "\033[0m" << std::endl;
}

Value StringLiteral::execute(Interpreter& interpreter)
{
  return ore_string(interpreter.heap(), m_value);
}

void NilLiteral::dump_impl(int indent) const
{
  print_indent(indent);
  std::cout << "\033[35m" << class_name() << " \033[33m@ {" << this << "} \033[0m" << std::endl;
}

Value NilLiteral::execute(Interpreter& interpreter)
{
  return ore_nil();
}

void BlockStatement::dump_impl(int indent) const
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

Value BlockStatement::execute(Interpreter& interpreter)
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

Value FunctionDeclaration::execute(Interpreter& interpreter)
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
      parameter_value.default_value = parameter.default_value.value()->execute(interpreter);
      if (interpreter.has_exception())
        return ore_nil();
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

Value CallExpression::execute(Interpreter& interpreter)
{
  Value value;
  if (m_callee->is_identifier()) {
    value = interpreter.get_variable(static_cast<Identifier&>(*m_callee).name());
  } else if (m_callee->is_member_expression()) {
    value = static_cast<MemberExpression&>(*m_callee).execute(interpreter);
  } else
    __builtin_unreachable();

  assert(value.is_object());

  auto* callee = value.as_object();

  if (callee->is_function()) {
    auto& function = static_cast<FunctionObject&>(*callee);

    assert(m_arguments.size() <= function.parameters().size());

    std::map<std::string, Value> passed_arguments;

    for (size_t i = 0; i < function.parameters().size(); ++i) {
      if (i < m_arguments.size()) {
        auto value = m_arguments[i]->execute(interpreter);
        if (interpreter.has_exception())
          return ore_nil();
        passed_arguments[function.parameters()[i].name] = value;
      } else {
        assert(function.parameters()[i].default_value.has_value());
        passed_arguments[function.parameters()[i].name] = function.parameters()[i].default_value.value();
      }
    }

    return interpreter.run(*function.body(), Interpreter::ScopeType::Function, passed_arguments);

  } else if (callee->is_native_function()) {
    auto& function = static_cast<NativeFunction&>(*callee);

    std::vector<Value> passed_arguments;
    for (auto& argument : m_arguments) {
      auto value = argument->execute(interpreter);
      if (interpreter.has_exception())
        return ore_nil();
      passed_arguments.push_back(value);
    }

    return function.native_function()(passed_arguments);
  }

  __builtin_unreachable();
}

void ReturnStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  argument().dump_impl(indent + 1);
}

Value ReturnStatement::execute(Interpreter& interpreter)
{
  auto argument_value = argument().execute(interpreter);
  interpreter.unwind_until(Interpreter::ScopeType::Function);
  return argument_value;
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

Value ForStatement::execute(Interpreter& interpreter)
{
  Value return_value;

  if (m_init.has_value()) {
    m_init.value()->execute(interpreter);
    if (interpreter.has_exception())
      return ore_nil();
  }

  bool test_value = true;
  if (m_test.has_value()) {
    test_value = m_test.value()->execute(interpreter).to_boolean();
    if (interpreter.has_exception())
      return ore_nil();
  }

  while (!interpreter.is_unwinding() && test_value) {
    return_value = m_body->execute(interpreter);
    if (interpreter.has_exception())
      return ore_nil();

    if (m_update.has_value()) {
      m_update.value()->execute(interpreter);
      if (interpreter.has_exception())
        return ore_nil();
    }

    test_value = m_test.value()->execute(interpreter).to_boolean();
    if (interpreter.has_exception())
      return ore_nil();
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

Value WhileStatement::execute(Interpreter& interpreter)
{

  Value return_value;

  while (!interpreter.is_unwinding() && test().execute(interpreter).to_boolean())
    return_value = body().execute(interpreter);

  return return_value;
}

void DoWhileStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  test().dump_impl(indent + 1);
  body().dump_impl(indent + 1);
}

Value DoWhileStatement::execute(Interpreter& interpreter)
{

  Value return_value;

  do {
    return_value = body().execute(interpreter);
  } while (!interpreter.is_unwinding() && test().execute(interpreter).to_boolean());

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
  printf("\033[35m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_lhs->dump_impl(indent + 1);
  m_rhs->dump_impl(indent + 1);
}

Value AssignmentExpression::execute(Interpreter& interpreter)
{
  auto value = m_rhs->execute(interpreter);
  if (interpreter.has_exception())
    return ore_nil();
  Value prev_value;
  PropertyKey key;
  Object* object;

  if (m_lhs->is_identifier()) {
    auto id = static_cast<Identifier&>(*m_lhs);
    key = id.name();

    if (m_op != Op::Assignment)
      prev_value = id.execute(interpreter);

  } else if (m_lhs->is_member_expression()) {
    auto& member_expression = static_cast<MemberExpression&>(*m_lhs);

    auto object_value = member_expression.object().execute(interpreter);
    object = object_value.to_object(interpreter.heap());

    if (member_expression.is_computed()) {
      auto computed_value = member_expression.property().execute(interpreter);
      if (computed_value.is_number())
        key = computed_value.as_number();
      else if (computed_value.is_string())
        key = computed_value.as_string()->string();
      else
        __builtin_unreachable();
    } else {
      assert(member_expression.property().is_identifier());
      auto& id = static_cast<Identifier&>(member_expression.property());
      key = id.name();
    }

    if (m_op != Op::Assignment)
      prev_value = object->get(key);

  } else {
    __builtin_unreachable();
  }

  switch (m_op) {
  case Op::AddAssignment:
    value = prev_value + value;
    break;
  case Op::SubAssignment:
    value = prev_value - value;
    break;
  case Op::MultAssignment:
    value = prev_value * value;
    break;
  case Op::DivAssignment:
    value = prev_value / value;
    break;
  case Op::ShiftLeftAssignment:
    value = prev_value << value;
    break;
  case Op::ShiftRightAssignment:
    value = prev_value >> value;
    break;
  case Op::ConcatAssignment:
    value = Value::string_concat(prev_value, value, interpreter.heap());
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

Value GlobalStatement::execute(Interpreter& interpreter)
{
  assert(m_assignment->lhs().is_identifier());
  auto& id = static_cast<Identifier&>(m_assignment->lhs());
  auto value = m_assignment->rhs().execute(interpreter);
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

Value UnaryExpression::execute(Interpreter& interpreter)
{
  auto value = m_operand->execute(interpreter);
  if (interpreter.has_exception())
    return ore_nil();
  switch (m_op) {
  case Op::Not:
    return !value;
  case Op::Length:
    return Value::length(value);
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

Value BinaryExpression::execute(Interpreter& interpreter)
{

  auto lhs_value = m_lhs->execute(interpreter);
  if (interpreter.has_exception())
    return ore_nil();

  if (m_op == Op::And && !lhs_value.to_boolean())
    return ore_boolean(false);
  else if (m_op == Op::Or && lhs_value.to_boolean())
    return ore_boolean(true);

  auto rhs_value = m_rhs->execute(interpreter);
  if (interpreter.has_exception())
    return ore_nil();

  switch (m_op) {
  case Op::Add: {
    return lhs_value + rhs_value;
  }
  case Op::Sub: {
    return lhs_value - rhs_value;
  }
  case Op::Mult: {
    return lhs_value * rhs_value;
  }
  case Op::Div: {
    return lhs_value / rhs_value;
  }
  case Op::Equals: {
    return lhs_value == rhs_value;
  }
  case Op::NotEquals: {
    return lhs_value != rhs_value;
  }
  case Op::GreaterThan: {
    return lhs_value > rhs_value;
  }
  case Op::LessThan: {
    return lhs_value < rhs_value;
  }
  case Op::GreaterThanOrEquals: {
    return lhs_value >= rhs_value;
  }
  case Op::LessThanOrEquals: {
    return lhs_value <= rhs_value;
  }
  case Op::ShiftLeft: {
    return lhs_value << rhs_value;
  }
  case Op::ShiftRight: {
    return lhs_value >> rhs_value;
  }
  case Op::StringConcat: {
    return Value::string_concat(lhs_value, rhs_value, interpreter.heap());
  }
  case Op::Xor: {
    return Value::logical_xor(lhs_value, rhs_value);
  }
  case Op::And: {
    return Value::logical_and(lhs_value, rhs_value);
  }
  case Op::Or: {
    return Value::logical_or(lhs_value, rhs_value);
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

Value MemberExpression::execute(Interpreter& interpreter)
{
  auto value = object().execute(interpreter);
  assert(value.is_object());
  auto obj = value.to_object(interpreter.heap());

  PropertyKey key;

  if (is_computed()) {
    auto computed_value = property().execute(interpreter);
    if (computed_value.is_number())
      key = PropertyKey(computed_value.as_number());
    else if (computed_value.is_string())
      key = PropertyKey(computed_value.as_string()->string());
    else
      __builtin_unreachable();

  } else {
    assert(property().is_identifier());
    auto id = static_cast<Identifier&>(property());
    key = PropertyKey(id.name());
  }

  if (obj->contains(key))
    return obj->get(key);

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

Value ObjectExpression::execute(Interpreter& interpreter)
{

  auto* object = interpreter.heap().allocate<Object>();

  for (auto& [key, value] : m_properties) {

    auto v = value->execute(interpreter);
    if (interpreter.has_exception())
      return ore_nil();
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

Value ArrayExpression::execute(Interpreter& interpreter)
{
  std::vector<Value> values;
  for (auto& element : m_elements) {
    auto value = element->execute(interpreter);
    if (interpreter.has_exception())
      return ore_nil();
    values.push_back(value);
  }

  return interpreter.heap().allocate<ArrayObject>(values);
}

void ExportStatement::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[0m\n", class_name(), this);
  m_argument->dump_impl(indent + 1);
}

Value ExportStatement::execute(Interpreter& interpreter)
{
  // FIXME: Actually implement exports.
  m_argument->execute(interpreter);
  return ore_nil();
}

void CatchClause::dump_impl(int indent) const
{
  print_indent(indent);
  printf("\033[32m%s \033[33m@ {%p}\033[34m %s \033[0m\n", class_name(), this, m_param.c_str());
  m_body->dump_impl(indent + 1);
}

Value CatchClause::execute(Interpreter& interpreter)
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

Value TryStatement::execute(Interpreter& interpreter)
{
  auto return_value = ore_nil();

  return_value = interpreter.run(*m_block, Interpreter::ScopeType::Try);
  if (interpreter.has_exception()) {
    ExceptionObject* exception = interpreter.exception();
    interpreter.clear_exception();

    std::map<std::string, Value> arguments;
    arguments[m_handler->param()] = exception;
    return_value = interpreter.run(m_handler->body(), Interpreter::ScopeType::Block, std::move(arguments));
  }
  if (m_finalizer.has_value())
    return_value = m_finalizer.value()->execute(interpreter);

  return return_value;
}

}
