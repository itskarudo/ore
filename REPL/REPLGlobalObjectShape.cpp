#include "REPLGlobalObjectShape.h"
#include <Config.h>
#include <filesystem>
#include <fmt/core.h>

REPLGlobalObjectShape::REPLGlobalObjectShape()
    : Ore::GlobalObjectShape()
{
  REGISTER_NATIVE_FUNCTION(print);
  REGISTER_NATIVE_FUNCTION(input);
  REGISTER_NATIVE_FUNCTION(import);
}

DEFINE_NATIVE_FUNCTION(REPLGlobalObjectShape::print)
{
  for (auto arg : params.args)
    std::cout << arg << std::endl;

  return Ore::ore_nil();
}

DEFINE_NATIVE_FUNCTION(REPLGlobalObjectShape::input)
{
  ARGS_SIZE_GUARD(print, 0);

  std::string input;
  std::getline(std::cin, input);

  return Ore::ore_string(params.interpreter.heap(), input);
}

DEFINE_NATIVE_FUNCTION(REPLGlobalObjectShape::import)
{
  ARGS_SIZE_GUARD(import, 1);
  ARG_TYPE_STRING(0);

  auto filename = params.args[0].as_string()->string();

  if (!filename.starts_with('.') && !filename.starts_with('/'))
    filename = ORE_MODULES_DIR + filename;

  if (!std::filesystem::exists(filename))
    return params.interpreter.throw_exception(Ore::ExceptionObject::file_not_found_exception(), fmt::format("no file named {}", filename));

  if (filename.ends_with(".so")) {
    auto* ffi_object = params.interpreter.heap().allocate<Ore::FFIObject>(filename);
    return Ore::Value(ffi_object);
  } else {
    ASSERT_NOT_REACHED();
  }
}
