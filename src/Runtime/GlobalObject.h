#pragma once

#include "Object.h"

namespace Ore {

class GlobalObject : public Object {
  public:
  GlobalObject();
  virtual ~GlobalObject() { }

  void initalize();
};

}
