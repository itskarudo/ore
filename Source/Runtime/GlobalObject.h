#pragma once

#include "Object.h"

namespace Ore {

class GlobalObject : public Object {
  public:
  GlobalObject();
  virtual ~GlobalObject() { }
  virtual char const* class_name() const override { return "GlobalObject"; }
  virtual ThrowResultOr<Value> get(PropertyKey) const override;
  bool contains(PropertyKey) const;
};

}
