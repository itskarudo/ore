#include "AST.h"
#include "Interpreter.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::CallExpression>("$gc");

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

