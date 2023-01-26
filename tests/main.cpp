#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::StringLiteral>("hello"));

  program.append<AST::BinaryExpression>(
      make_unique<AST::BooleanLiteral>(true),
      AST::BinaryExpression::Op::Or,
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("print"),
          std::move(args)));

  program.dump();
  interpreter.run(program);
}