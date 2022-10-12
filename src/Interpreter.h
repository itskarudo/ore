#pragma once

#include "GC/Heap.h"
#include "Runtime/Object.h"
#include <vector>

namespace Ore {
class Interpreter {

  public:
  Interpreter();

  struct ScopeFrame {
    AST::BlockStatement& block;
    std::map<std::string, Value> variables;
  };

  const GC::Heap& heap() const { return m_heap; }
  GC::Heap& heap() { return m_heap; }

  Object const& global_object() const { return m_global_object; }
  Object& global_object() { return m_global_object; }

  Value run(AST::BlockStatement&);

  ScopeFrame& current_scope() { return m_scope_frames.back(); }

  void enter_scope(AST::BlockStatement&);
  void leave_scope();

  Value get_variable(std::string const& name) const;
  void set_variable(std::string const& name, Value);

  void collect_roots(std::vector<GC::Cell*>& roots);

  private:
  GC::Heap m_heap;
  Object m_global_object;
  std::vector<ScopeFrame> m_scope_frames;
};

}
