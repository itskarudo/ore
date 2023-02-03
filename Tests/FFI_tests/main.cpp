#include <Ore.h>

Ore::Value greet_name(Ore::GC::Heap& heap, std::vector<Ore::Value>& args)
{
  std::cout << "hello " << args[0].as_string()->string() << std::endl;
  return Ore::ore_number(69);
}

ORE_INIT_FUNC OreInitialize(std::vector<OreExportEntry>& exports)
{
  exports.push_back({ "greet_name", greet_name });
}
