#pragma once

#include "AST.h"
#include "Forward.h"
#include "GC/Cell.h"
#include "GC/Heap.h"
#include "GC/HeapBlock.h"
#include "Interpreter.h"
#include "Parser/Lexer.h"
#include "Parser/Token.h"
#include "Runtime/ArrayObject.h"
#include "Runtime/BooleanObject.h"
#include "Runtime/FFIObject.h"
#include "Runtime/FunctionObject.h"
#include "Runtime/GlobalObject.h"
#include "Runtime/NativeFunction.h"
#include "Runtime/NumberObject.h"
#include "Runtime/Object.h"
#include "Runtime/PrimitiveString.h"
#include "Runtime/PropertyKey.h"
#include "Runtime/StringObject.h"
#include "Runtime/Value.h"

#define ORE_INIT_FUNC extern "C" void

struct OreFuncParams {
  Ore::Interpreter& interpreter;
  std::vector<Ore::Value>& args;
};

using OreFunctionDecl = Ore::ThrowResultOr<Ore::Value> (*)(OreFuncParams);

struct OreExportEntry {
  char const* name;
  OreFunctionDecl decl;
};

#define ARGS_SIZE_GUARD(func_name, args_size)                                                                                           \
  do {                                                                                                                                  \
    if (params.args.size() != args_size)                                                                                                \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), #func_name "() requires " #args_size " arguments."); \
  } while (0)

#define ARG_TYPE_NUMBER(index)                                                                                              \
  do {                                                                                                                      \
    if (!params.args[index].is_number())                                                                                    \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument " #index " is not a number."); \
  } while (0)

#define ARG_TYPE_STRING(index)                                                                                              \
  do {                                                                                                                      \
    if (!params.args[index].is_string())                                                                                    \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument " #index " is not a string."); \
  } while (0)

#define ARG_TYPE_BOOLEAN(index)                                                                                              \
  do {                                                                                                                       \
    if (!params.args[index].is_boolean())                                                                                    \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument " #index " is not a boolean."); \
  } while (0)

#define ARG_TYPE_OBJECT(index)                                                                                               \
  do {                                                                                                                       \
    if (!params.args[index].is_object())                                                                                     \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument " #index " is not an object."); \
  } while (0)

#define ARG_TYPE_NIL(index)                                                                                            \
  do {                                                                                                                 \
    if (!params.args[index].is_nil())                                                                                  \
      return params.interpreter.throw_exception(ExceptionObject::type_exception(), "argument " #index " is not nil."); \
  } while (0)
