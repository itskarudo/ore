#pragma once

#include "Object.h"
namespace Ore {
class ArrayObjectShape : public Object {
  public:
  ArrayObjectShape();
  virtual char const* class_name() const override { return "ArrayObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(append);
};
}
