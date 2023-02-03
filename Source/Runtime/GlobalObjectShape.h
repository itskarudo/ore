#pragma once
#include "Object.h"

namespace Ore {
class GlobalObjectShape : public Object {
  public:
  GlobalObjectShape();
  virtual char const* class_name() const override { return "GlobalObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(print);
  DECLARE_NATIVE_FUNCTION(input);
  DECLARE_NATIVE_FUNCTION(gc);
  DECLARE_NATIVE_FUNCTION(import);
};
}
