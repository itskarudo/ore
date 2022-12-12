#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  /**
   * print(import("./libffi_test").greet_name("karudo"));
   */

  std::vector<std::unique_ptr<AST::Expression>> import_args;
  import_args.push_back(make_unique<AST::Literal>(ore_string(interpreter.heap(), "./libffi_test")));

  std::vector<std::unique_ptr<AST::Expression>> greet_args;
  greet_args.push_back(make_unique<AST::Literal>(ore_string(interpreter.heap(), "karudo")));

  std::vector<std::unique_ptr<AST::Expression>> print_args;
  print_args.push_back(make_unique<AST::CallExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::CallExpression>(
              make_unique<AST::Identifier>("import"),
              std::move(import_args)),
          make_unique<AST::Identifier>("greet_name")),
      std::move(greet_args)));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(print_args));

  program.dump();
  interpreter.run(program);
}
