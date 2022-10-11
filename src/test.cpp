#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::UnaryExpression>(
      AST::UnaryExpression::Op::Length,
      make_unique<AST::Literal>(
          ore_string(interpreter.heap(), "foo")));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

