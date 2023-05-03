#include "GlobalObjectShape.h"
#include "ExceptionObject.h"
#include "FFIObject.h"
#include <Config.h>
#include <filesystem>
#include <fmt/core.h>

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

  if (filename.starts_with('.') || filename.starts_with('/')) {
    if (!std::filesystem::exists(filename))
      return interpreter.throw_exception(ExceptionObject::file_not_found_exception(), fmt::format("no file named %s", filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = interpreter.heap().allocate<FFIObject>(filename);
      return Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  } else {
    auto full_filename = ORE_MODULES_DIR + filename;
    if (!std::filesystem::exists(full_filename))
      return interpreter.throw_exception(ExceptionObject::file_not_found_exception(), fmt::format("no file named %s", full_filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = interpreter.heap().allocate<FFIObject>(full_filename);
      return Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  }
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::throw)
{
  assert(args.size() == 2);
  assert(args[0].is_string());
  assert(args[1].is_string());

  return interpreter.throw_exception(args[0].as_string()->string(), args[1].as_string()->string());
}

}
