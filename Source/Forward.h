#pragma once

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
}

#define DECLARE_NATIVE_FUNCTION(name) \
  static Value name##_native_function(Interpreter&, std::vector<Value>&);

#define DEFINE_NATIVE_FUNCTION(name) \
  Value name##_native_function(Interpreter& interpreter, std::vector<Value>& args)

#define REGISTER_NATIVE_FUNCTION(name) \
  put_native_function(PropertyKey(#name), name##_native_function);

#define ENUMERATE_OBJECT_SHAPES                                \
  __ENUM_OBJECT_SHAPES(global_object_shape, GlobalObjectShape) \
  __ENUM_OBJECT_SHAPES(array_object_shape, ArrayObjectShape)
