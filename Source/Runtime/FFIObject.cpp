#include "FFIObject.h"
#include "../Ore.h"
#include <dlfcn.h>

namespace Ore {
FFIObject::FFIObject(std::string const& filename)
{
  m_handle = dlopen(filename.c_str(), RTLD_LAZY);
  assert(m_handle != nullptr);

  auto* init_addr = dlsym(m_handle, "OreInitialize");
  assert(init_addr != nullptr);

  auto* init_func = reinterpret_cast<void (*)(std::vector<OreExportEntry>&)>(init_addr);
  std::vector<OreExportEntry> exports;
  init_func(exports);

  for (auto& entry : exports)
    // FIXME: why does put() and put_native_function() cause a stack overflow?
    m_properties[entry.name] = heap().allocate<NativeFunction>([this, entry](Interpreter&, std::vector<Value> args) {
      return entry.decl(heap(), args);
    });
}

FFIObject::~FFIObject()
{
  dlclose(m_handle);
}

void FFIObject::put(PropertyKey key, Value value)
{
  __builtin_unreachable();
}
}
