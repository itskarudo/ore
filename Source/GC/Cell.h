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
  Interpreter& interpreter() const;

  class Visitor {
public:
    virtual void visit(Cell*) = 0;
  };

  virtual void visit_graph(Visitor& visitor) { }

  virtual char const* class_name() const = 0;

  friend std::ostream& operator<<(std::ostream& os, Cell const* cell);

  private:
  bool m_mark { false };
  bool m_live { true };
};
}
