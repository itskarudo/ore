#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  /**
   * function foo() {
   *   global sloop = 69
   * }
   * foo()
   * print(sloop)
   */

  auto body = make_unique<AST::BlockStatement>();
  body->append<AST::GlobalStatement>(
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("sloop"),
          make_unique<AST::Literal>(69)));

  program.append<AST::FunctionDeclaration>("foo", std::move(body));
  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("foo"));

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Identifier>("sloop"));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);
}
