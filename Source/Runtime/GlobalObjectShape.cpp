#include "GlobalObjectShape.h"
#include "FFIObject.h"
#include <filesystem>

namespace Ore {
GlobalObjectShape::GlobalObjectShape()
{
  REGISTER_NATIVE_FUNCTION(print);
  REGISTER_NATIVE_FUNCTION(input);
  REGISTER_NATIVE_FUNCTION(gc);
  REGISTER_NATIVE_FUNCTION(import);
  REGISTER_NATIVE_FUNCTION(throw);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::print)
{
  for (auto arg : args)
    std::cout << arg << std::endl;

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::input)
{
  std::string input;
  std::getline(std::cin, input);

  return ore_string(interpreter.heap(), input);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::gc)
{
  interpreter.heap().collect_garbage();
  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::import)
{
  assert(args.size() == 1);

  auto filename = args[0].as_string()->string();

  // TODO: add the ability to import ore source files as well.
  // TODO: have a global static path to look for installed libraries in.
  // TODO: add support for other platforms (windows DLLs).
  auto full_filename = filename + ".so";
  if (!std::filesystem::exists(full_filename))
    return ore_nil();

  auto* ffi_object = interpreter.heap().allocate<FFIObject>(full_filename);
  return Value(ffi_object);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::throw)
{
  assert(args.size() == 2);
  assert(args[0].is_string());
  assert(args[1].is_string());

  interpreter.throw_exception(args[0].as_string()->string(), args[1].as_string()->string());
  return ore_nil();
}

}
