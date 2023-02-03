#pragma once

#include "Cell.h"
#include "Heap.h"
#include <concepts>
#include <stdint.h>
#include <unistd.h>

namespace Ore::GC {
class HeapBlock {
  public:
  HeapBlock(Heap&, size_t cell_size);

  static constexpr size_t block_size = 16 * 1024;
  static HeapBlock* from_cell(Cell const* cell) { return reinterpret_cast<HeapBlock*>((uintptr_t)cell & ~(block_size - 1)); }

  size_t cell_size() const { return m_cell_size; }
  size_t cell_count() const { return (block_size - sizeof(HeapBlock)) / m_cell_size; }
  Heap& heap() const { return m_heap; }

  Cell* cell(size_t index) { return reinterpret_cast<Cell*>(&m_storage[index * cell_size()]); }
  Cell* allocate();
  void deallocate(Cell*);

  template<typename Callback>
    requires std::invocable<Callback, Cell*>
  void for_each_cell(Callback callback)
  {
    for (size_t i = 0; i < cell_count(); ++i)
      callback(cell(i));
  }

  private:
  struct FreelistEntry : public Cell {
    FreelistEntry* next;
  };

  FreelistEntry* m_freelist { nullptr };
  Heap& m_heap;
  size_t m_cell_size;
  uint8_t m_storage[];
};
}
