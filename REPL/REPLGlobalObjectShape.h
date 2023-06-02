#pragma once

#include <Config.h>
#include <Ore.h>

class REPLGlobalObjectShape final : public Ore::GlobalObjectShape {
  public:
  REPLGlobalObjectShape();
  virtual char const* class_name() const override { return "REPLGlobalObjectShape"; }

  private:
  DECLARE_NATIVE_FUNCTION(print);
  DECLARE_NATIVE_FUNCTION(input);

#if ON_UNIX
  DECLARE_NATIVE_FUNCTION(import);
#endif
};
