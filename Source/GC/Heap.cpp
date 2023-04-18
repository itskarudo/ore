#include "Heap.h"
#include "../Interpreter.h"
#include "HeapBlock.h"
#include <Config.h>
#include <csetjmp>
#include <pthread.h>
#include <set>

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
  LivenessVisitor(std::vector<Cell*>& roots)
  {
    for (auto* root : roots) {
      root->set_marked(true);
      m_work_queue.push_back(root);
    }
  }

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
    m_work_queue.push_back(cell);
  }

  void mark_live_cells()
  {
    while (!m_work_queue.empty()) {
      auto* last_element = m_work_queue.back();
      m_work_queue.pop_back();
      last_element->visit_graph(*this);
    }
  }

  private:
  std::vector<Cell*> m_work_queue;
};

void Heap::collect_garbage(CollectionType collection_type)
{
  std::vector<Cell*> roots;

  // mark all live cells
  if (collection_type == CollectionType::Garbage) {
    if (m_interpreter.global_object() != nullptr)
      roots.push_back(m_interpreter.global_object());
  }

  collect_conservative_roots(roots);
  m_interpreter.collect_roots(roots, collection_type);

  if constexpr (HEAP_DEBUG)
    for (auto* root : roots)
      std::cout << "\033[32m! GC: Root -> " << root << "\033[0m" << std::endl;

  auto visitor = LivenessVisitor(roots);

  visitor.mark_live_cells();

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

void Heap::collect_conservative_roots(std::vector<Cell*>& roots)
{
  jmp_buf buf;
  setjmp(buf);

  std::set<uintptr_t> possible_pointers;

  auto stack_bottom = get_stack_bottom();
  auto stack_reference = std::bit_cast<uintptr_t>(&buf);

  for (uintptr_t i = stack_reference; i < stack_bottom; i += sizeof(uintptr_t)) {
    auto data = *reinterpret_cast<uintptr_t*>(i);
    possible_pointers.insert(data);
  }

  std::set<HeapBlock*> heap_blocks;

  for_each_heap_block([&](HeapBlock* block) {
    heap_blocks.insert(block);
  });

  for (auto ptr : possible_pointers) {
    if (!ptr)
      continue;

    auto* possible_block = HeapBlock::from_cell(reinterpret_cast<Cell*>(ptr));
    if (heap_blocks.contains(possible_block)) {
      if (auto* cell = possible_block->cell_from_possible_pointer(ptr))
        roots.push_back(cell);
    }
  }
}

uintptr_t Heap::get_stack_bottom()
{
  uintptr_t base;
  size_t size;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_getattr_np(pthread_self(), &attr);

  pthread_attr_getstack(&attr, (void**)&base, &size);

  pthread_attr_destroy(&attr);

  return base + size;
}

}
