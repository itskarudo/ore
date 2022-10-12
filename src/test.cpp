#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::IfStatement>(
      make_unique<AST::Literal>(true),
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("$gc")),
      make_unique<AST::BlockStatement>());

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

