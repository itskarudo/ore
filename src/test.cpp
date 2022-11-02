#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Value.h"

using std::make_unique;
using namespace Ore;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  for (int i = 0; i < 1024; ++i)
    program.append<AST::ObjectExpression>();

  program.dump();

  interpreter.run(program);
}

