#pragma once

#include <type_traits>

#include "GC/Heap.h"
#include "Runtime/ArrayObjectShape.h"
#include "Runtime/ExceptionObject.h"
#include "Runtime/GlobalObject.h"
#include "Runtime/GlobalObjectShape.h"
#include "Runtime/NumberObjectShape.h"
#include "Runtime/Object.h"
#include "Runtime/Result.h"
#include <vector>

namespace Ore {

class Interpreter {

  public:
  template<typename GlobalObjectType>
    requires(std::is_base_of_v<GlobalObjectShape, GlobalObjectType>)
  static std::unique_ptr<Interpreter> create()
  {
    auto interpreter = std::unique_ptr<Ore::Interpreter>(new Interpreter());

    interpreter->set_global_object_shape(interpreter->heap().allocate<GlobalObjectType>());

    interpreter->m_global_object = interpreter->m_heap.allocate<GlobalObject>();

    return interpreter;
  }

  enum class ScopeType {
    Block,
    Function
  };

  struct FunctionScope {
    std::string name;
  };

  struct ScopeFrame {
    ScopeType type;
    AST::BlockStatement& block;
    std::map<std::string, Value> variables;
  };

  const GC::Heap& heap() const { return m_heap; }
  GC::Heap& heap() { return m_heap; }

  GlobalObject const* global_object() const { return m_global_object; }
  GlobalObject* global_object() { return m_global_object; }

  Result run(AST::BlockStatement&, std::map<std::string, Value> const& arguments = {}, ScopeType = ScopeType::Block, std::optional<std::string> function_name = {});

  ScopeFrame& current_scope() { return m_scope_frames.back(); }

  Result throw_exception(std::string const& type, std::string const& message);

  void enter_scope(AST::BlockStatement&, std::map<std::string, Value> const& arguments, ScopeType);
  void leave_scope();

  std::vector<FunctionScope>& function_scopes() { return m_function_scopes; }

  void push_function_scope(FunctionScope scope)
  {
    m_function_scopes.push_back(scope);
  }
  void pop_function_scope()
  {
    m_function_scopes.pop_back();
  }

  ThrowResultOr<Value> get_variable(std::string const& name);
  void set_variable(std::string const& name, Value);

  void collect_roots(std::vector<GC::Cell*>& roots, GC::Heap::CollectionType);

  Value block_result() { return m_block_result; }
  void set_block_result(Value block_result) { m_block_result = block_result; }

#define __ENUM_OBJECT_SHAPES(name, ObjectName)              \
  ObjectName* name()                                        \
  {                                                         \
    if (m_object_shapes.name == nullptr)                    \
      m_object_shapes.name = m_heap.allocate<ObjectName>(); \
    return m_object_shapes.name;                            \
  }                                                         \
  void set_##name(ObjectName* object)                       \
  {                                                         \
    m_object_shapes.name = object;                          \
  }

  ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES

  private:
  Interpreter()
      : m_heap(*this)
  {
  }

  GC::Heap m_heap;

  GlobalObject* m_global_object { nullptr };

  struct {
#define __ENUM_OBJECT_SHAPES(name, ObjectName) \
  ObjectName* name;

    ENUMERATE_OBJECT_SHAPES
#undef __ENUM_OBJECT_SHAPES
  } m_object_shapes = { nullptr };

  std::vector<ScopeFrame> m_scope_frames;
  std::vector<FunctionScope> m_function_scopes;
  Value m_block_result;
};
}
