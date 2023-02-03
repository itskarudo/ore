#include "GlobalObjectShape.h"
#include "FFIObject.h"
#include <filesystem>

namespace Ore {
GlobalObjectShape::GlobalObjectShape()
{
  put_native_function(PropertyKey("print"), print);
  put_native_function(PropertyKey("input"), input);
  put_native_function(PropertyKey("gc"), gc);
  put_native_function(PropertyKey("import"), import);
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

}
