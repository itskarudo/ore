#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  /**
   * i = 0
   * do
   *   print(i)
   * while (i < 10)
   */

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Literal>(
      ore_string(interpreter.heap(), "this should get printed once")));

  auto body = make_unique<AST::BlockStatement>();
  body->append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.append<AST::DoWhileStatement>(
      make_unique<AST::Literal>(false),
      std::move(body));

  program.dump();
  interpreter.run(program);
}
