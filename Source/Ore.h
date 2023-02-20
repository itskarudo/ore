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

using OreFunctionDecl = Ore::Value (*)(OreFuncParams);

struct OreExportEntry {
  char const* name;
  OreFunctionDecl decl;
};
