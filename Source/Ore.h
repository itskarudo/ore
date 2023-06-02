#pragma once

#include "AST.h"
#include "Forward.h"
#include "GC/Cell.h"
#include "GC/Heap.h"
#include "GC/HeapBlock.h"
#include "Interpreter.h"
#include "Parser/Lexer.h"
#include "Parser/RDParser.h"
#include "Parser/Token.h"
#include "Runtime/ArrayObject.h"
#include "Runtime/BooleanObject.h"
#include "Runtime/FunctionObject.h"
#include "Runtime/GlobalObject.h"
#include "Runtime/NativeFunction.h"
#include "Runtime/NumberObject.h"
#include "Runtime/Object.h"
#include "Runtime/PrimitiveString.h"
#include "Runtime/PropertyKey.h"
#include "Runtime/StringObject.h"
#include "Runtime/Value.h"
#include <Config.h>

#if ON_UNIX
#  include "Runtime/FFIObject.h"
#endif

using OreFunctionDecl = Ore::ThrowResultOr<Ore::Value> (*)(OreFuncParams);

#define ORE_INIT_FUNC(exports) extern "C" void OreInitialize(std::map<char const*, OreFunctionDecl>& exports)
