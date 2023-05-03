#include <Ore.h>
#include <math.h>

namespace Ore {

ThrowResultOr<Value> cos(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "cos() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::cos(n));
}

ThrowResultOr<Value> sin(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "sin() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::sin(n));
}

ThrowResultOr<Value> tan(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "tan() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::tan(n));
}

ThrowResultOr<Value> arccos(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arccos() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::acos(n));
}

ThrowResultOr<Value> arcsin(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arcsin() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::asin(n));
}

ThrowResultOr<Value> arctan(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arctan() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::atan(n));
}

ThrowResultOr<Value> cosh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "cosh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::cosh(n));
}

ThrowResultOr<Value> sinh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "sinh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::sinh(n));
}

ThrowResultOr<Value> tanh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "tanh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::tanh(n));
}

ThrowResultOr<Value> arccosh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arccosh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::acosh(n));
}

ThrowResultOr<Value> arcsinh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arcsinh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::asinh(n));
}

ThrowResultOr<Value> arctanh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arctanh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::atanh(n));
}

ThrowResultOr<Value> abs(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "abs() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return (n > 0) ? ore_number(n) : ore_number(-n);
}

ThrowResultOr<Value> exp(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "exp() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::exp(n));
}

ThrowResultOr<Value> log(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "log() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::log(n));
}

ThrowResultOr<Value> log2(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "log2() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::log2(n));
}

ThrowResultOr<Value> log10(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "log10() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::log10(n));
}

ThrowResultOr<Value> sqrt(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "sqrt() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::sqrt(n));
}

ThrowResultOr<Value> cbrt(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "cbrt() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

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
