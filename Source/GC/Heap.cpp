#include "Heap.h"
#include "../Interpreter.h"
#include "HeapBlock.h"
#include <Config.h>

namespace Ore::GC {

Heap::~Heap()
{
  collect_garbage(CollectionType::Everything);
  for (auto* it : m_blocks)
    free(it);
}

Cell* Heap::allocate_cell(size_t cell_size)
{
  if (m_allocations_since_last_gc > m_max_allocations_between_gcs) {
    m_allocations_since_last_gc = 0;
    collect_garbage();
  } else
    m_allocations_since_last_gc++;

  for (auto& block : m_blocks) {
    if (cell_size > block->cell_size())
      continue;
    if (auto* cell = block->allocate())
      return cell;
  }

  auto* block = (HeapBlock*)aligned_alloc(HeapBlock::block_size, HeapBlock::block_size);

  if (block == nullptr) {
    interpreter().throw_exception(ExceptionObject::out_of_memory_exception(), "could not allocate heap block");
    collect_garbage();
    return nullptr;
  }

  new (block) HeapBlock(*this, cell_size);
  m_blocks.push_back(block);
  return block->allocate();
}

class LivenessVisitor : public Cell::Visitor {
  public:
  LivenessVisitor() { }

  virtual void visit(Cell* cell) override
  {
    if (cell->marked()) {
      if constexpr (HEAP_DEBUG)
        std::cout << "\033[35m# GC: Cyclic dependency -> " << cell << "\033[0m" << std::endl;
      return;
    }

    if constexpr (HEAP_DEBUG)
      std::cout << "\033[33m? GC: Marked -> " << cell << "\033[0m" << std::endl;
    cell->set_marked(true);
    cell->visit_graph(*this);
  }
};

void Heap::collect_garbage(CollectionType collection_type)
{
  LivenessVisitor visitor;
  std::vector<Cell*> roots;

  // mark all live cells
  if (collection_type == CollectionType::Garbage) {
    if (m_interpreter.global_object() != nullptr)
      roots.push_back(m_interpreter.global_object());
  }

  m_interpreter.collect_roots(roots, collection_type);

  if constexpr (HEAP_DEBUG)
    for (auto* root : roots)
      std::cout << "\033[32m! GC: Root -> " << root << "\033[0m" << std::endl;

  for (auto* root : roots) {
    visitor.visit(root);
  }

  // sweep dead cells and reset mark bit for alive cells
  for (auto& block : m_blocks) {
    block->for_each_cell([&](Cell* cell) {
      if (cell->live()) {
        if (cell->marked())
          cell->set_marked(false);
        else {
          if constexpr (HEAP_DEBUG)
            std::cout << "\033[31m~ GC: Freeing -> " << cell << "\033[0m" << std::endl;
          block->deallocate(cell);
        }
      }
    });
  }
}

}
