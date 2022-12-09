#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Literal>(69));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  interpreter.run(program);
}
