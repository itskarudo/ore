#pragma once

#include "../Forward.h"
#include "HeapBlock.h"
#include <memory>
#include <vector>

namespace Ore::GC {
class Heap {
  public:
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

  void collect_garbage(bool collect_global_object = false);

  private:
  Cell* allocate_cell(size_t);

  void visit_reachable_cells(std::vector<Cell*>& roots, std::vector<Cell*>& live_cells);

  std::vector<HeapBlock*> m_blocks;
  Interpreter& m_interpreter;

  size_t m_allocations_since_last_gc { 0 };
  size_t m_max_allocations_between_gcs { 1024 };
};
}
