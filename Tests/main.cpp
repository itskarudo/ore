#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> print_args;
  print_args.push_back(make_unique<AST::BinaryExpression>(
      make_unique<AST::NumberLiteral>(4),
      AST::BinaryExpression::Op::Pow,
      make_unique<AST::NumberLiteral>(0.5)));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(print_args));

  program.dump();
  auto retval = interpreter.run(program);

  if (retval.is_exception()) {
    auto* exception = static_cast<ExceptionObject*>(retval.value().as_object());
    std::cout << "Uncaught Exception(" << exception->type() << "): " << exception->message() << std::endl;
  }
}
