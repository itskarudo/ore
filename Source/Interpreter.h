#pragma once

#include "GC/Heap.h"
#include "Runtime/ExceptionObject.h"
#include "Runtime/GlobalObject.h"
#include "Runtime/GlobalObjectShape.h"
#include "Runtime/Object.h"
#include <vector>

namespace Ore {
class Interpreter {

  public:
  Interpreter();

  enum class ScopeType {
    None,
    Block,
    Function,
    Try
  };

  struct ScopeFrame {
    AST::BlockStatement& block;
    std::map<std::string, Value> variables;
    ScopeType type;
  };

  const GC::Heap& heap() const { return m_heap; }
  GC::Heap& heap() { return m_heap; }

  GlobalObject const* global_object() const { return m_global_object; }
  GlobalObject* global_object() { return m_global_object; }

  Value run(AST::BlockStatement&, ScopeType type = ScopeType::Block, std::map<std::string, Value> const& arguments = {});

  ScopeFrame& current_scope() { return m_scope_frames.back(); }

  void unwind_until(ScopeType scope_type) { m_unwind_until = scope_type; }
  bool is_unwinding() const { return m_unwind_until != ScopeType::None; }

  Value throw_exception(std::string const& message);
  void clear_exception() { m_exception = nullptr; }
  bool has_exception() const { return m_exception != nullptr; }
  ExceptionObject* exception() { return m_exception; }

  void enter_scope(AST::BlockStatement&, ScopeType, std::map<std::string, Value> const& arguments);
  void leave_scope();

  Value get_variable(std::string const& name);
  void set_variable(std::string const& name, Value);

  void collect_roots(std::vector<GC::Cell*>& roots);

#define __ENUM_OBJECT_SHAPES(name, ObjectName) \
  ObjectName* name() const                     \
  {                                            \
    return m_object_shapes.name;               \
  }

  ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES

  private:
  GC::Heap m_heap;

  GlobalObject* m_global_object;
  ExceptionObject* m_exception { nullptr };

  struct {
#define __ENUM_OBJECT_SHAPES(name, ObjectName) \
  ObjectName* name;

    ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES
  } m_object_shapes;

  std::vector<ScopeFrame> m_scope_frames;
  ScopeType m_unwind_until { ScopeType::None };
};

}
