#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> import_args;
  import_args.push_back(make_unique<AST::StringLiteral>("./libffi_test"));

  std::vector<std::unique_ptr<AST::Expression>> greet_args;
  greet_args.push_back(make_unique<AST::StringLiteral>("karu"));

  program.append<AST::CallExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::CallExpression>(
              make_unique<AST::Identifier>("import"),
              std::move(import_args)),
          make_unique<AST::Identifier>("greet_name")),
      std::move(greet_args));

  program.dump();
  interpreter.run(program);
  if (interpreter.has_exception()) {
    std::cout << "Uncaught Exception(" << interpreter.exception()->type() << "): " << interpreter.exception()->message() << std::endl;
  }
}
