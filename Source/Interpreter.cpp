#include "Interpreter.h"
#include "AST.h"
#include "Runtime/NativeFunction.h"
#include "Runtime/Value.h"

namespace Ore {

Interpreter::Interpreter()
    : m_heap(*this)
{
#define __ENUM_OBJECT_SHAPES(name, ObjectName) \
  m_object_shapes.name = m_heap.allocate<ObjectName>();

  ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES

  m_global_object = m_heap.allocate<GlobalObject>();
}

void Interpreter::enter_scope(AST::BlockStatement& scope_frame, ScopeType type, std::map<std::string, Value> const& arguments)
{
  m_scope_frames.push_back({ scope_frame, arguments, type });
}

void Interpreter::leave_scope()
{
  m_scope_frames.pop_back();
}

Value Interpreter::get_variable(std::string const& name)
{
  for (auto frame = m_scope_frames.rbegin(); frame != m_scope_frames.rend(); ++frame)
    if (frame->variables.contains(name))
      return frame->variables.at(name);

  if (global_object()->contains(name))
    return global_object()->get(name);

  return throw_exception(ExceptionObject::reference_exception(), "Unknown Identifier");
}

void Interpreter::set_variable(std::string const& name, Value value)
{
  for (auto frame = m_scope_frames.rbegin(); frame != m_scope_frames.rend(); ++frame)
    if (frame->variables.contains(name)) {
      frame->variables[name] = value;
      return;
    }

  if (global_object()->contains(name)) {
    global_object()->put(name, value);
    return;
  }

  current_scope().variables[name] = value;
}

Value Interpreter::run(AST::BlockStatement& block, ScopeType type, std::map<std::string, Value> const& arguments)
{

  enter_scope(block, type, arguments);

  Value return_value = ore_nil();

  for (auto& child : block.children()) {
    auto last_value = child->execute(*this);

    if (m_unwind_until == ScopeType::Function)
      return_value = last_value;

    if (is_unwinding())
      break;
  }

  if (type == m_unwind_until)
    m_unwind_until = ScopeType::None;

  leave_scope();

  return return_value;
}

void Interpreter::collect_roots(std::vector<GC::Cell*>& roots)
{
  for (auto& frame : m_scope_frames) {
    for (auto& [name, value] : frame.variables) {
      if (value.is_object())
        roots.push_back(value.as_object());
    }
  }
}

Value Interpreter::throw_exception(std::string const& type, std::string const& message)
{
  m_exception = heap().allocate<ExceptionObject>(std::move(type), std::move(message));
  unwind_until(ScopeType::Try);
  return ore_nil();
}

}
