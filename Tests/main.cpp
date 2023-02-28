#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("x"),
      AST::AssignmentExpression::Op::Assignment,
      std::make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("input")));

  std::vector<std::unique_ptr<AST::Expression>> args;

  args.push_back(make_unique<AST::BinaryExpression>(
      make_unique<AST::StringLiteral>("you entered: "),
      AST::BinaryExpression::Op::StringConcat,
      make_unique<AST::Identifier>("x")));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);

  if (interpreter.has_exception()) {
    std::cout << "Uncaught Exception(" << interpreter.exception()->type() << "): " << interpreter.exception()->message() << std::endl;
  }
}
