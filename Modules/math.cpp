#include <Ore.h>
#include <math.h>

namespace Ore::Math {

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

ThrowResultOr<Value> floor(OreFuncParams params)
{
  ARGS_SIZE_GUARD(floor, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::floor(n));
}

ThrowResultOr<Value> ceil(OreFuncParams params)
{
  ARGS_SIZE_GUARD(ceil, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::ceil(n));
}

ThrowResultOr<Value> round(OreFuncParams params)
{
  ARGS_SIZE_GUARD(round, 1);
  ARG_TYPE_NUMBER(0);

  auto n = params.args[0].as_number();
  return ore_number(std::round(n));
}

}

ORE_INIT_FUNC(exports)
{
  exports["cos"] = Ore::Math::cos;
  exports["sin"] = Ore::Math::sin;
  exports["tan"] = Ore::Math::tan;
  exports["arccos"] = Ore::Math::arccos;
  exports["arcsin"] = Ore::Math::arcsin;
  exports["arctan"] = Ore::Math::arctan;
  exports["cosh"] = Ore::Math::cosh;
  exports["sinh"] = Ore::Math::sinh;
  exports["tanh"] = Ore::Math::tanh;
  exports["arccosh"] = Ore::Math::arccosh;
  exports["arcsinh"] = Ore::Math::arcsinh;
  exports["arctanh"] = Ore::Math::arctanh;
  exports["abs"] = Ore::Math::abs;
  exports["exp"] = Ore::Math::exp;
  exports["log"] = Ore::Math::log;
  exports["log2"] = Ore::Math::log2;
  exports["log10"] = Ore::Math::log10;
  exports["sqrt"] = Ore::Math::sqrt;
  exports["cbrt"] = Ore::Math::cbrt;
  exports["floor"] = Ore::Math::floor;
  exports["ceil"] = Ore::Math::ceil;
  exports["round"] = Ore::Math::round;
}
