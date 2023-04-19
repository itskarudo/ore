#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  program.append<AST::BinaryExpression>(
      make_unique<AST::StringLiteral>("this should throw"),
      AST::BinaryExpression::Op::Add,
      make_unique<AST::NumberLiteral>(5));

  program.dump();
  auto retval = interpreter.run(program);

  if (retval.is_exception()) {
    auto* exception = static_cast<ExceptionObject*>(retval.value().as_object());
    std::cout << "Uncaught Exception(" << exception->type() << "): " << exception->message() << std::endl;
  }
}
