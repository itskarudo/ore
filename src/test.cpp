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
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("$gc"),
          make_unique<AST::Identifier>("foo")),
      make_unique<AST::Literal>(5));

  program.append<AST::ReturnStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::MemberExpression>(
              make_unique<AST::Identifier>("$gc"),
              make_unique<AST::Identifier>("foo")),
          AST::BinaryExpression::Op::Add,
          make_unique<AST::Literal>(1)));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

