#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  // for (i = 0; i < 10; i += 1) {
  //   if (i == 5) break
  //   print(i)
  // }

  auto for_body = std::make_unique<AST::BlockStatement>();
  for_body->append<AST::IfStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::Equals,
          make_unique<AST::NumberLiteral>(5)),
      make_unique<AST::BreakStatement>(),
      make_unique<AST::BlockStatement>());

  std::vector<std::unique_ptr<AST::Expression>> print_args;
  print_args.push_back(make_unique<AST::Identifier>("i"));

  for_body->append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(print_args));

  program.append<AST::ForStatement>(
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("i"),
          AST::AssignmentExpression::Op::Assignment,
          make_unique<AST::NumberLiteral>(0)),
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::LessThan,
          make_unique<AST::NumberLiteral>(10)),
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("i"),
          AST::AssignmentExpression::Op::AddAssignment,
          make_unique<AST::NumberLiteral>(1)),
      std::move(for_body));

  program.dump();
  auto retval = interpreter.run(program);

  if (retval.is_exception()) {
    auto* exception = static_cast<ExceptionObject*>(retval.value().as_object());
    std::cout << "Uncaught Exception(" << exception->type() << "): " << exception->message() << std::endl;
  }
}
