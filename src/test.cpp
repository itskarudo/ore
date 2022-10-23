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

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("foo"),
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Literal>(true),
          AST::BinaryExpression::Op::Xor,
          make_unique<AST::Literal>(false)));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

