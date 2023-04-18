#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  // root = {}
  // r = root
  // while(true) {
  //    r.next = {};
  //    r = r.next;
  // }

  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("root"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::ObjectExpression>());

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("r"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::Identifier>("root"));

  auto block = make_unique<AST::BlockStatement>();

  block->append<AST::AssignmentExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("r"),
          make_unique<AST::Identifier>("next")),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::ObjectExpression>());

  block->append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("r"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("r"),
          make_unique<AST::Identifier>("next")));

  program.append<AST::WhileStatement>(
      std::make_unique<AST::BooleanLiteral>(true),
      std::move(block));

  program.dump();
  auto retval = interpreter.run(program);

  if (retval.is_exception()) {
    auto* exception = static_cast<ExceptionObject*>(retval.value().as_object());
    std::cout << "Uncaught Exception(" << exception->type() << "): " << exception->message() << std::endl;
  }
}
