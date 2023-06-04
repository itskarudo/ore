#pragma once

#include "Object.h"
namespace Ore {
class NumberObjectShape : public Object {
  public:
  NumberObjectShape();
  virtual char const* class_name() const override { return "NumberObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(to_string);
};
}
