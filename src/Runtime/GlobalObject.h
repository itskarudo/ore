#pragma once

#include "Object.h"

namespace Ore {

class GlobalObject : public Object {
  public:
  GlobalObject(Interpreter&);
  virtual ~GlobalObject() { }

  void initalize();

  private:
  Interpreter& m_interpreter;
};

}
