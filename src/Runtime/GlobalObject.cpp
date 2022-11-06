#include "GlobalObject.h"
#include "../Interpreter.h"
#include "NativeFunction.h"

namespace Ore {
GlobalObject::GlobalObject(Interpreter& interpreter)
    : m_interpreter(interpreter)
{
  initalize();
}

void GlobalObject::initalize()
{
  put(std::string("print"), m_interpreter.heap().allocate<NativeFunction>([&](std::vector<Value> args) {
    for (auto arg : args) {
      if (arg.is_string())
        printf("%s\n", arg.as_string()->string().c_str());
      else if (arg.is_number())
        printf("%s\n", std::to_string(arg.as_number()).c_str());
      else if (arg.is_boolean())
        printf("%s\n", arg.as_boolean() ? "true" : "false");
      else
        __builtin_unreachable();
    }

    return ore_nil();
  }));

  put(std::string("$gc"), m_interpreter.heap().allocate<NativeFunction>([&](std::vector<Value>) {
    m_interpreter.heap().collect_garbage();
    return ore_nil();
  }));
}

}
