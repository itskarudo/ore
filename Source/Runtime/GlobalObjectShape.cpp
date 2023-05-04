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
  for (auto arg : params.args)
    std::cout << arg << std::endl;

  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::input)
{
  ARGS_SIZE_GUARD(print, 0);

  std::string input;
  std::getline(std::cin, input);

  return ore_string(params.interpreter.heap(), input);
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::gc)
{
  ARGS_SIZE_GUARD(print, 0);

  params.interpreter.heap().collect_garbage();
  return ore_nil();
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::import)
{
  ARGS_SIZE_GUARD(import, 1);
  ARG_TYPE_STRING(0);

  auto filename = params.args[0].as_string()->string();

  if (filename.starts_with('.') || filename.starts_with('/')) {
    if (!std::filesystem::exists(filename))
      return params.interpreter.throw_exception(ExceptionObject::file_not_found_exception(), fmt::format("no file named {}", filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = params.interpreter.heap().allocate<FFIObject>(filename);
      return Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  } else {
    auto full_filename = ORE_MODULES_DIR + filename;
    if (!std::filesystem::exists(full_filename))
      return params.interpreter.throw_exception(ExceptionObject::file_not_found_exception(), fmt::format("no file named {}", full_filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = params.interpreter.heap().allocate<FFIObject>(full_filename);
      return Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  }
}

DEFINE_NATIVE_FUNCTION(GlobalObjectShape::throw)
{
  ARGS_SIZE_GUARD(throw, 2);
  ARG_TYPE_STRING(0);
  ARG_TYPE_STRING(1);

  return params.interpreter.throw_exception(params.args[0].as_string()->string(), params.args[1].as_string()->string());
}

}
