#include "AST.h"
#include "Interpreter.h"

using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::BinaryExpression>(
      std::make_unique<AST::Literal>(60),
      AST::BinaryExpression::Op::Div,
      std::make_unique<AST::Literal>(0));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

