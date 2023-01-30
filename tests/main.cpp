#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> elements;
  elements.push_back(make_unique<AST::StringLiteral>("hello"));
  elements.push_back(make_unique<AST::NumberLiteral>(5));

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::ArrayExpression>(std::move(elements)));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);
}
