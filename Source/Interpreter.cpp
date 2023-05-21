#include "Interpreter.h"
#include "AST.h"
#include "Runtime/NativeFunction.h"
#include "Runtime/Value.h"

namespace Ore {

void Interpreter::enter_scope(AST::BlockStatement& scope_frame, std::map<std::string, Value> const& arguments, ScopeType type)
{
  m_scope_frames.push_back({ type, scope_frame, arguments });
}

void Interpreter::leave_scope()
{
  auto top_scope = m_scope_frames.back();
  m_scope_frames.pop_back();
  if (top_scope.type == ScopeType::Function)
    m_function_scopes.pop_back();
}

ThrowResultOr<Value> Interpreter::get_variable(std::string const& name)
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

  if (m_scope_frames.size() == 0)
    global_object()->put(name, value);
  else
    current_scope().variables[name] = value;
}

Result Interpreter::run(AST::BlockStatement& block, std::map<std::string, Value> const& arguments, ScopeType type, std::optional<std::string> function_name)
{

  if (!block.is_program())
    enter_scope(block, arguments, type);

  if (type == ScopeType::Function) {
    assert(function_name.has_value());
    push_function_scope({ function_name.value() });
  }

  Result return_value = ore_nil();

  for (auto& child : block.children()) {
    auto last_value = child->execute(*this);

    if (last_value.type() != Result::Type::Normal) {
      return_value = last_value;
      break;
    } else {
      set_block_result(last_value.value());
    }
  }

  if (!block.is_program())
    leave_scope();

  return return_value;
}

void Interpreter::collect_roots(std::vector<GC::Cell*>& roots, GC::Heap::CollectionType collection_type)
{
  for (auto& frame : m_scope_frames) {
    for (auto& [name, value] : frame.variables) {
      if (value.is_object())
        roots.push_back(value.as_object());
    }
  }

  if (collection_type == GC::Heap::CollectionType::Garbage) {

#define __ENUM_OBJECT_SHAPES(name, ObjectName) \
  if (m_object_shapes.name != nullptr)         \
    roots.push_back(m_object_shapes.name);

    ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES
  }
}

Result Interpreter::throw_exception(std::string const& type, std::string const& message)
{
  return {
    Result::Type::Throw,
    heap().allocate<ExceptionObject>(std::move(type), std::move(message))
  };
}

}
