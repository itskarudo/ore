#pragma once

#include <stddef.h>

namespace Ore {

class SourceRange {
  public:
  struct Position {
    size_t line { 1 };
    size_t column { 0 };
  };

  SourceRange() = default;

  SourceRange(Position start, Position end)
      : m_start(start)
      , m_end(end)
  {
  }

  private:
  Position m_start;
  Position m_end;
};

}
