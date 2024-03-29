#include "HeapBlock.h"
#include <utility>

namespace Ore::GC {
HeapBlock::HeapBlock(Heap& heap, size_t cell_size)
    : m_heap(heap)
    , m_cell_size(cell_size)
{
  for (int i = 0; i < cell_count(); ++i) {
    auto* entry = reinterpret_cast<FreelistEntry*>(cell(i));
    entry->set_live(false);
    if (i == cell_count() - 1)
      entry->next = nullptr;
    else
      entry->next = reinterpret_cast<FreelistEntry*>(cell(i + 1));
  }

  m_freelist = static_cast<FreelistEntry*>(cell(0));
}

Cell* HeapBlock::allocate()
{
  if (!m_freelist)
    return nullptr;

  return std::exchange(m_freelist, m_freelist->next);
}

void HeapBlock::deallocate(Cell* cell)
{
  cell->set_live(false);
  cell->~Cell();
  auto* entry = reinterpret_cast<FreelistEntry*>(cell);
  entry->next = m_freelist;
  m_freelist = entry;
}

Cell* HeapBlock::cell_from_possible_pointer(uintptr_t ptr)
{
  if (ptr < reinterpret_cast<uintptr_t>(m_storage))
    return nullptr;

  size_t index = (ptr - reinterpret_cast<uintptr_t>(m_storage)) / m_cell_size;
  if (index >= cell_count())
    return nullptr;

  return cell(index);
}

}
