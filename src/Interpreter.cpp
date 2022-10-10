#include "Interpreter.h"
#include "AST.h"

namespace Ore {

Interpreter::Interpreter()
    : m_heap(*this)
{
}

void Interpreter::enter_scope(AST::ScopeNode& scope_frame)
{
  m_scope_frames.push_back({ scope_frame, {} });
}

void Interpreter::leave_scope()
{
  m_scope_frames.pop_back();
  heap().collect_garbage();
}

Value Interpreter::get_variable(std::string const& name) const
{
  for (auto frame = m_scope_frames.rbegin(); frame != m_scope_frames.rend(); ++frame)
    if (frame->variables.count(name))
      return frame->variables.at(name);

  if (global_object().contains(name))
    return global_object().get(name);

  return Value();
}

void Interpreter::set_variable(std::string const& name, Value value)
{
  for (auto frame = m_scope_frames.rbegin(); frame != m_scope_frames.rend(); ++frame)
    if (frame->variables.count(name)) {
      frame->variables[name] = value;
      return;
    }

  if (global_object().contains(name)) {
    global_object().put(name, value);
    return;
  }

  current_scope().variables[name] = value;
}

Value Interpreter::run(AST::ScopeNode& scope_node)
{

  enter_scope(scope_node);

  Value return_value;

  for (auto child : scope_node.children()) {
    return_value = child->execute(*this);
  }

  leave_scope();

  return return_value;
}

void Interpreter::collect_roots(std::vector<GC::Cell*>& roots)
{

  roots.push_back(&global_object());

  for (auto& frame : m_scope_frames) {
    for (auto& [name, value] : frame.variables) {
      if (value.is_object())
        roots.push_back(value.as_object());
    }
  }
}

}
