#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::StringLiteral>("./libffi_test.so"));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("import"),
      std::move(args));

  program.dump();
  auto retval = interpreter.run(program);

  if (retval.is_exception()) {
    auto* exception = static_cast<ExceptionObject*>(retval.value().as_object());
    std::cout << "Uncaught Exception(" << exception->type() << "): " << exception->message() << std::endl;
  }
}
