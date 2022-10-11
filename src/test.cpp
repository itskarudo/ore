#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("foo"),
      make_unique<AST::Literal>(
          interpreter.heap().allocate<PrimitiveString>("manama jeff")));

  program.append<AST::ReturnStatement>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("foo"),
          make_unique<AST::Identifier>("length")));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

