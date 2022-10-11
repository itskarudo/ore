#include "Heap.h"
#include "../Interpreter.h"
#include "HeapBlock.h"

namespace Ore::GC {
Cell* Heap::allocate_cell(size_t cell_size)
{

  for (auto& block : m_blocks) {
    if (cell_size > block->cell_size())
      continue;
    if (auto* cell = block->allocate())
      return cell;
  }

  auto* block = (HeapBlock*)malloc(HeapBlock::block_size);
  new (block) HeapBlock(cell_size);
  m_blocks.push_back(block);
  return block->allocate();
}

class LivenessVisitor : public Cell::Visitor {
  public:
  LivenessVisitor() { }

  virtual void did_visit(Cell* cell) const override
  {
    if (cell->marked())
      return;
    cell->set_marked(true);
  }
};

void Heap::collect_garbage(bool collect_global_object)
{
  LivenessVisitor visitor;
  std::vector<Cell*> roots;

  // mark all live cells
  if (!collect_global_object)
    roots.push_back(&m_interpreter.global_object());

  m_interpreter.collect_roots(roots);

  for (auto* root : roots)
    std::cout << "\033[32m! GC: Root -> " << root << "\033[0m" << std::endl;

  for (auto* root : roots) {
    root->visit_graph(visitor);
  }

  // sweep dead cells and reset mark bit for alive cells
  for (auto& block : m_blocks) {
    block->for_each_cell([&](Cell* cell) {
      if (cell->live()) {
        if (cell->marked())
          cell->set_marked(false);
        else {
          std::cout << "\033[31m~ GC: Freeing -> " << cell << "\033[0m" << std::endl;
          block->deallocate(cell);
        }
      }
    });
  }
}

}
