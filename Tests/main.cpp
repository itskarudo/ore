#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("foo"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::NumberLiteral>(1));

  auto block = make_unique<AST::BlockStatement>();
  block->append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("bar"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::NumberLiteral>(2));

  program.append(std::move(block));

  program.dump();
  interpreter.run(program);
  if (interpreter.has_exception()) {
    std::cout << "Uncaught Exception(" << interpreter.exception()->type() << "): " << interpreter.exception()->message() << std::endl;
  }
}
