#include <Ore.h>
#include <ctime>

namespace Ore::Time {

ThrowResultOr<Value> time(OreFuncParams params)
{
  ARGS_SIZE_GUARD(time, 0);

  return ore_number(std::time(nullptr));
}

ThrowResultOr<Value> sleep(OreFuncParams params)
{
  ARGS_SIZE_GUARD(sleep, 1);
  ARG_TYPE_NUMBER(0);

  int secs = params.args[0].as_number();
  ::sleep(secs);
  return ore_nil();
}

}

ORE_INIT_FUNC(exports)
{
  exports["time"] = Ore::Time::time;
  exports["sleep"] = Ore::Time::sleep;
}
