#pragma once

#include "../Forward.h"
#include "HeapBlock.h"
#include <memory>
#include <vector>

namespace Ore::GC {
class Heap {
  public:
  enum class CollectionType {
    Garbage,
    Everything
  };

  Heap(Interpreter& interpreter)
      : m_interpreter(interpreter)
  {
  }
  ~Heap();

  template<typename T, typename... Args>
  T* allocate(Args&&... args)
  {
    auto* memory = allocate_cell(sizeof(T));
    new (memory) T(std::forward<Args>(args)...);
    return static_cast<T*>(memory);
  }

  void collect_garbage(CollectionType = CollectionType::Garbage);

  Interpreter& interpreter() const { return m_interpreter; }

  void set_debug_heap(bool v) { m_debug_heap = v; }
  void set_gc_on_every_allocation(bool v) { m_gc_on_every_allocation = v; }

  private:
  Cell* allocate_cell(size_t);
  void collect_conservative_roots(std::vector<Cell*>&);
  uintptr_t get_stack_bottom();

  template<typename Callback>
    requires std::invocable<Callback, HeapBlock*>
  void for_each_heap_block(Callback callback)
  {
    for (auto* block : m_blocks)
      callback(block);
  }

  std::vector<HeapBlock*> m_blocks;
  Interpreter& m_interpreter;
  bool m_debug_heap { false };
  bool m_gc_on_every_allocation { false };

  size_t m_allocations_since_last_gc { 0 };
  size_t m_max_allocations_between_gcs { 1024 };

  friend class LivenessVisitor;
};
}
