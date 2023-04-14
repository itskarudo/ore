#include <Ore.h>

Ore::Result greet_name(OreFuncParams params)
{
  std::cout << "hello " << params.args[0].as_string()->string() << std::endl;
  return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "idk lol");
}

ORE_INIT_FUNC OreInitialize(std::vector<OreExportEntry>& exports)
{
  exports.push_back({ "greet_name", greet_name });
}
