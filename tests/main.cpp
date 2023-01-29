#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::StringLiteral>("hello"));
  args.push_back(make_unique<AST::NumberLiteral>(5));
  args.push_back(make_unique<AST::BooleanLiteral>(true));
  args.push_back(make_unique<AST::NilLiteral>());
  args.push_back(make_unique<AST::ObjectExpression>());

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);
}
