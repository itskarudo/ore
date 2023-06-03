#pragma once
#include "Object.h"

namespace Ore {
class GlobalObjectShape : public Object {
  public:
  GlobalObjectShape();
  virtual char const* class_name() const override { return "GlobalObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(gc);
  DECLARE_NATIVE_FUNCTION(assert);
  DECLARE_NATIVE_FUNCTION(throw);
  DECLARE_NATIVE_FUNCTION(all);
  DECLARE_NATIVE_FUNCTION(any);
  DECLARE_NATIVE_FUNCTION(type);
};
}
