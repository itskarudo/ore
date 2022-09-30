#pragma once

#include "Cell.h"
#include <stdint.h>
#include <unistd.h>

namespace Ore::GC {
class HeapBlock {
  public:
  HeapBlock(size_t cell_size);

  static constexpr size_t block_size = 16 * 1024;

  size_t cell_size() const { return m_cell_size; }
  size_t cell_count() const { return (block_size - sizeof(HeapBlock)) / m_cell_size; }

  Cell* cell(size_t index) { return reinterpret_cast<Cell*>(&m_storage[index * cell_size()]); }
  Cell* allocate();
  void deallocate(Cell*);

  template<typename Callback>
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
  size_t m_cell_size;
  uint8_t m_storage[];
};
}
