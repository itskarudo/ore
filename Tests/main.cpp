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
      make_unique<AST::ArrayExpression>());

  std::vector<std::unique_ptr<AST::Expression>> ap_args;
  ap_args.push_back(make_unique<AST::Identifier>("x"));
  ap_args.push_back(make_unique<AST::NumberLiteral>(69));

  program.append<AST::CallExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("x"),
          make_unique<AST::Identifier>("append")),
      std::move(ap_args));

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Identifier>("x"));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);

  if (interpreter.has_exception()) {
    std::cout << "Uncaught Exception(" << interpreter.exception()->type() << "): " << interpreter.exception()->message() << std::endl;
  }
}
