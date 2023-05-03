#include <Ore.h>
#include <math.h>

namespace Ore {

ThrowResultOr<Value> cos(OreFuncParams params)
{
  ARGS_SIZE_GUARD(cos, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::cos(n));
}

ThrowResultOr<Value> sin(OreFuncParams params)
{
  ARGS_SIZE_GUARD(sin, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::sin(n));
}

ThrowResultOr<Value> tan(OreFuncParams params)
{
  ARGS_SIZE_GUARD(tan, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::tan(n));
}

ThrowResultOr<Value> arccos(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arccos, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::acos(n));
}

ThrowResultOr<Value> arcsin(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arcsin, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::asin(n));
}

ThrowResultOr<Value> arctan(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arctan, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::atan(n));
}

ThrowResultOr<Value> cosh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(cosh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::cosh(n));
}

ThrowResultOr<Value> sinh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(sinh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::sinh(n));
}

ThrowResultOr<Value> tanh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(tanh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::tanh(n));
}

ThrowResultOr<Value> arccosh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arccosh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::acosh(n));
}

ThrowResultOr<Value> arcsinh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arcsinh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::asinh(n));
}

ThrowResultOr<Value> arctanh(OreFuncParams params)
{
  ARGS_SIZE_GUARD(arctanh, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::atanh(n));
}

ThrowResultOr<Value> abs(OreFuncParams params)
{
  ARGS_SIZE_GUARD(abs, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return (n > 0) ? ore_number(n) : ore_number(-n);
}

ThrowResultOr<Value> exp(OreFuncParams params)
{
  ARGS_SIZE_GUARD(exp, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::exp(n));
}

ThrowResultOr<Value> log(OreFuncParams params)
{
  ARGS_SIZE_GUARD(log, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::log(n));
}

ThrowResultOr<Value> log2(OreFuncParams params)
{
  ARGS_SIZE_GUARD(log2, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::log2(n));
}

ThrowResultOr<Value> log10(OreFuncParams params)
{
  ARGS_SIZE_GUARD(log10, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::log10(n));
}

ThrowResultOr<Value> sqrt(OreFuncParams params)
{
  ARGS_SIZE_GUARD(sqrt, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::sqrt(n));
}

ThrowResultOr<Value> cbrt(OreFuncParams params)
{
  ARGS_SIZE_GUARD(cbrt, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::cbrt(n));
}

}

ORE_INIT_FUNC OreInitialize(std::vector<OreExportEntry>& exports)
{
  exports.push_back({ "cos", Ore::cos });
  exports.push_back({ "sin", Ore::sin });
  exports.push_back({ "tan", Ore::tan });
  exports.push_back({ "arccos", Ore::arccos });
  exports.push_back({ "arcsin", Ore::arcsin });
  exports.push_back({ "arctan", Ore::arctan });
  exports.push_back({ "cosh", Ore::cosh });
  exports.push_back({ "sinh", Ore::sinh });
  exports.push_back({ "tanh", Ore::tanh });
  exports.push_back({ "arccosh", Ore::arccosh });
  exports.push_back({ "arcsinh", Ore::arcsinh });
  exports.push_back({ "arctanh", Ore::arctanh });
  exports.push_back({ "abs", Ore::abs });
  exports.push_back({ "exp", Ore::exp });
  exports.push_back({ "log", Ore::log });
  exports.push_back({ "log2", Ore::log2 });
  exports.push_back({ "log10", Ore::log10 });
  exports.push_back({ "sqrt", Ore::sqrt });
  exports.push_back({ "cbrt", Ore::cbrt });
}
