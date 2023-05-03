#include <Ore.h>
#include <math.h>

namespace Ore {

Ore::ThrowResultOr<Ore::Value> cos(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "cos() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::cos(n));
}

Ore::ThrowResultOr<Ore::Value> sin(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "sin() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::sin(n));
}

Ore::ThrowResultOr<Ore::Value> tan(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "tan() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::tan(n));
}

Ore::ThrowResultOr<Ore::Value> arccos(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arccos() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::acos(n));
}

Ore::ThrowResultOr<Ore::Value> arcsin(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arcsin() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::asin(n));
}

Ore::ThrowResultOr<Ore::Value> arctan(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arctan() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::atan(n));
}

Ore::ThrowResultOr<Ore::Value> cosh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "cosh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::cosh(n));
}

Ore::ThrowResultOr<Ore::Value> sinh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "sinh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::sinh(n));
}

Ore::ThrowResultOr<Ore::Value> tanh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "tanh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::tanh(n));
}

Ore::ThrowResultOr<Ore::Value> arccosh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arccosh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::acosh(n));
}

Ore::ThrowResultOr<Ore::Value> arcsinh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arcsinh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::asinh(n));
}

Ore::ThrowResultOr<Ore::Value> arctanh(OreFuncParams params)
{
  if (params.args.size() != 1)
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "arctanh() requires 1 argument.");

  if (!params.args[0].is_number())
    return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument is not a number.");

  auto n = params.args[0].as_number();
  return ore_number(std::atanh(n));
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
}
