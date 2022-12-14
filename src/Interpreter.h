#pragma once

#include "GC/Heap.h"
#include "Runtime/GlobalObject.h"
#include "Runtime/Object.h"
#include <vector>

namespace Ore {
class Interpreter {

  public:
  Interpreter();

  enum class ScopeType {
    Block,
    Function
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
  void do_return() { m_do_return = true; }
  bool is_returning() const { return m_do_return; }

  void enter_scope(AST::BlockStatement&, ScopeType, std::map<std::string, Value> const& arguments);
  void leave_scope();

  Value get_variable(std::string const& name) const;
  void set_variable(std::string const& name, Value);

  void collect_roots(std::vector<GC::Cell*>& roots);

  private:
  GC::Heap m_heap;
  GlobalObject* m_global_object;
  std::vector<ScopeFrame> m_scope_frames;
  bool m_do_return { false };
};

}
