#pragma once

#include "Object.h"
namespace Ore {
class ArrayObjectShape : public Object {
  public:
  ArrayObjectShape();
  virtual char const* class_name() const override { return "ArrayObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(append);
  DECLARE_NATIVE_FUNCTION(pop);
  DECLARE_NATIVE_FUNCTION(insert);
  DECLARE_NATIVE_FUNCTION(remove);
  DECLARE_NATIVE_FUNCTION(map);
  DECLARE_NATIVE_FUNCTION(filter);
};
}
