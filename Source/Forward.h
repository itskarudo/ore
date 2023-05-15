#pragma once
#include <vector>

namespace Ore {

namespace AST {
class ASTNode;
class BlockStatement;
};

namespace GC {
class Heap;
class HeapBlock;
class Cell;
}

class Interpreter;
class Value;
class Object;
class PrimitiveString;

class Result;
template<typename T>
class ThrowResultOr;
}

struct OreFuncParams {
  Ore::Interpreter& interpreter;
  std::vector<Ore::Value>& args;
};

#define DECLARE_NATIVE_FUNCTION(name) \
  static Ore::ThrowResultOr<Ore::Value> name##_native_function(OreFuncParams);

#define DEFINE_NATIVE_FUNCTION(name) \
  Ore::ThrowResultOr<Ore::Value> name##_native_function(OreFuncParams params)

#define REGISTER_NATIVE_FUNCTION(name) \
  put_native_function(Ore::PropertyKey(#name), name##_native_function);

#define ENUMERATE_OBJECT_SHAPES                                \
  __ENUM_OBJECT_SHAPES(global_object_shape, GlobalObjectShape) \
  __ENUM_OBJECT_SHAPES(array_object_shape, ArrayObjectShape)

#define ARGS_SIZE_GUARD(func_name, args_size)                                                                                                \
  do {                                                                                                                                       \
    if (params.args.size() != args_size)                                                                                                     \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), #func_name "() requires " #args_size " arguments."); \
  } while (0)

#define ARG_TYPE_NUMBER(index)                                                                                                   \
  do {                                                                                                                           \
    if (!params.args[index].is_number())                                                                                         \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "argument " #index " is not a number."); \
  } while (0)

#define ARG_TYPE_STRING(index)                                                                                                   \
  do {                                                                                                                           \
    if (!params.args[index].is_string())                                                                                         \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "argument " #index " is not a string."); \
  } while (0)

#define ARG_TYPE_BOOLEAN(index)                                                                                                   \
  do {                                                                                                                            \
    if (!params.args[index].is_boolean())                                                                                         \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "argument " #index " is not a boolean."); \
  } while (0)

#define ARG_TYPE_OBJECT(index)                                                                                                    \
  do {                                                                                                                            \
    if (!params.args[index].is_object())                                                                                          \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "argument " #index " is not an object."); \
  } while (0)

#define ARG_TYPE_NIL(index)                                                                                                 \
  do {                                                                                                                      \
    if (!params.args[index].is_nil())                                                                                       \
      return params.interpreter.throw_exception(Ore::ExceptionObject::type_exception(), "argument " #index " is not nil."); \
  } while (0)
