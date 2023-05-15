#include "ScriptGlobalObjectShape.h"
#include <Config.h>
#include <filesystem>
#include <fmt/core.h>

ScriptGlobalObjectShape::ScriptGlobalObjectShape()
    : Ore::GlobalObjectShape()
{
  REGISTER_NATIVE_FUNCTION(print);
  REGISTER_NATIVE_FUNCTION(input);
  REGISTER_NATIVE_FUNCTION(import);
}

DEFINE_NATIVE_FUNCTION(ScriptGlobalObjectShape::print)
{
  for (auto arg : params.args)
    std::cout << arg << std::endl;

  return Ore::ore_nil();
}

DEFINE_NATIVE_FUNCTION(ScriptGlobalObjectShape::input)
{
  ARGS_SIZE_GUARD(print, 0);

  std::string input;
  std::getline(std::cin, input);

  return Ore::ore_string(params.interpreter.heap(), input);
}

DEFINE_NATIVE_FUNCTION(ScriptGlobalObjectShape::import)
{
  ARGS_SIZE_GUARD(import, 1);
  ARG_TYPE_STRING(0);

  auto filename = params.args[0].as_string()->string();

  if (filename.starts_with('.') || filename.starts_with('/')) {
    if (!std::filesystem::exists(filename))
      return params.interpreter.throw_exception(Ore::ExceptionObject::file_not_found_exception(), fmt::format("no file named {}", filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = params.interpreter.heap().allocate<Ore::FFIObject>(filename);
      return Ore::Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  } else {
    auto full_filename = ORE_MODULES_DIR + filename;
    if (!std::filesystem::exists(full_filename))
      return params.interpreter.throw_exception(Ore::ExceptionObject::file_not_found_exception(), fmt::format("no file named {}", full_filename));

    if (filename.ends_with(".so")) {
      auto* ffi_object = params.interpreter.heap().allocate<Ore::FFIObject>(full_filename);
      return Ore::Value(ffi_object);
    } else {
      // TODO: implement importing ore source file.
      assert(false);
    }
  }
}
