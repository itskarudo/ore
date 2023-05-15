#pragma once

#include <Ore.h>

class ScriptGlobalObjectShape final : public Ore::GlobalObjectShape {
  public:
  ScriptGlobalObjectShape();
  virtual char const* class_name() const override { return "ScriptGlobalObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(print);
  DECLARE_NATIVE_FUNCTION(input);
  DECLARE_NATIVE_FUNCTION(import);
};
