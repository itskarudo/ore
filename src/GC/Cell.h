#pragma once

#include "../Forward.h"
#include <iostream>

namespace Ore::GC {
class Cell {
  public:
  virtual ~Cell() { }

  bool marked() const { return m_mark; }
  void set_marked(bool m) { m_mark = m; }

  bool live() const { return m_live; }
  void set_live(bool l) { m_live = l; }

  Heap& heap() const;

  class Visitor {
public:
    virtual void did_visit(Cell*) const = 0;
  };

  virtual void visit_graph(Visitor& visitor)
  {
    std::cout << "\033[33m? GC: Marked -> " << this << "\033[0m" << std::endl;
    visitor.did_visit(this);
  }

  virtual char const* class_name() const = 0;

  friend std::ostream& operator<<(std::ostream& os, Cell const* cell);

  private:
  bool m_mark { false };
  bool m_live { true };
};
}
