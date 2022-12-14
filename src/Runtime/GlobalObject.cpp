#include "GlobalObject.h"
#include "../Interpreter.h"
#include "FFIObject.h"
#include "NativeFunction.h"
#include <filesystem>

namespace Ore {
GlobalObject::GlobalObject()
{
  initalize();
}

void GlobalObject::initalize()
{
  put_native_function(PropertyKey("print"), [&](std::vector<Value> args) {
    for (auto arg : args) {
      if (arg.is_string())
        printf("%s\n", arg.as_string()->string().c_str());
      else if (arg.is_number())
        printf("%s\n", std::to_string(arg.as_number()).c_str());
      else if (arg.is_boolean())
        printf("%s\n", arg.as_boolean() ? "true" : "false");
      else
        __builtin_unreachable();
    }

    return ore_nil();
  });

  put_native_function(PropertyKey("input"), [&](std::vector<Value>) {
    std::string input;
    std::getline(std::cin, input);

    return ore_string(GC::HeapBlock::from_cell(this)->heap(), input);
  });

  put_native_function(PropertyKey("$gc"), [&](std::vector<Value>) {
    GC::HeapBlock::from_cell(this)->heap().collect_garbage();
    return ore_nil();
  });

  put_native_function(PropertyKey("import"), [&](std::vector<Value> args) {
    assert(args.size() == 1);

    auto filename = args[0].as_string()->string();

    // TODO: add the ability to import ore source files as well.
    // TODO: have a global static path to look for installed libraries in.
    // TODO: add support for other platforms (windows DLLs).
    auto full_filename = filename + ".so";
    if (!std::filesystem::exists(full_filename))
      return ore_nil();

    auto* ffi_object = GC::HeapBlock::from_cell(this)->heap().allocate<FFIObject>(full_filename);
    return Value(ffi_object);
  });
}

}
