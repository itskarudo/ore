#pragma once

#include "../Ore.h"
#include "NativeFunction.h"
#include "Object.h"
#include <map>

namespace Ore {
class FFIObject final : public Object {
  public:
  FFIObject(std::string const& filename);
  virtual ~FFIObject();

  virtual char const* class_name() const override { return "FFIObject"; }

  virtual void put(PropertyKey key, Value value) override;

  private:
  void* m_handle;
};
}
