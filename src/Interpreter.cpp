#include "Interpreter.h"
#include "AST.h"
#include "Runtime/NativeFunction.h"
#include "Runtime/Value.h"

namespace Ore {

Interpreter::Interpreter()
    : m_heap(*this)
{
  global_object().put(std::string("print"), heap().allocate<NativeFunction>([&](std::vector<Value> args) {
    for (auto arg : args) {
      if (arg.is_string())
        printf("%s\n", arg.as_string()->string().c_str());
      else if (arg.is_number())
        printf("%s\n", std::to_string(arg.as_number()).c_str());
      else if (arg.is_boolean())
        printf("%s\n", arg.as_boolean() ? "true" : "false");
      else
        __builtin_unreachable();
    }

    return ore_nil();
  }));

  global_object().put(std::string("$gc"), heap().allocate<NativeFunction>([&](std::vector<Value>) {
    heap().collect_garbage();
    return ore_nil();
  }));
}

void Interpreter::enter_scope(AST::BlockStatement& scope_frame, ScopeType type, std::map<std::string, Value> const& arguments)
{
  m_scope_frames.push_back({ scope_frame, arguments, type });
}

void Interpreter::leave_scope()
{
  m_scope_frames.pop_back();
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

Value Interpreter::run(AST::BlockStatement& block, ScopeType type, std::map<std::string, Value> const& arguments)
{

  enter_scope(block, type, arguments);

  Value return_value = ore_nil();

  for (auto& child : block.children()) {
    auto last_value = child->execute(*this);
    if (m_do_return) {
      return_value = last_value;
      break;
    }
  }

  if (type == ScopeType::Function && m_do_return)
    m_do_return = false;

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

}
