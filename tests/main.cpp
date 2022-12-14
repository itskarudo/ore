#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  /**
   * function test() {
   *   i = 0;
   *   while (i < 10)
   *   {
   *     if (i == 5) return i;
   *     i++;
   *   }
   * }
   * print(test());
   */

  auto body = make_unique<AST::BlockStatement>();
  body->append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("i"),
      make_unique<AST::Literal>(0));

  auto while_body = make_unique<AST::BlockStatement>();

  while_body->append<AST::IfStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::Equals,
          make_unique<AST::Literal>(5)),
      make_unique<AST::ReturnStatement>(
          make_unique<AST::Identifier>("i")),
      make_unique<AST::BlockStatement>());

  while_body->append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("i"),
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::Add,
          make_unique<AST::Literal>(1)));

  body->append<AST::WhileStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::NotEquals,
          make_unique<AST::Literal>(10)),
      std::move(while_body));

  program.append<AST::FunctionDeclaration>(
      "test",
      std::move(body));

  std::vector<std::unique_ptr<AST::Expression>> print_args;
  print_args.push_back(make_unique<AST::CallExpression>(
      make_unique<AST::Identifier>("test")));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(print_args));

  program.dump();
  interpreter.run(program);
}
