#include <Ore.h>
#include <math.h>

namespace Ore {

Ore::ThrowResultOr<Ore::Value> cos(OreFuncParams params)
{
  assert(params.args.size() == 1);
  assert(params.args[0].is_number());

  auto n = params.args[0].as_number();
  return ore_number(std::cos(n));
}

}

ORE_INIT_FUNC OreInitialize(std::vector<OreExportEntry>& exports)
{
  exports.push_back({ "cos", Ore::cos });
}
