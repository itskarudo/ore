#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  /**
   * for (i = 0; i < 10; i++)
   *   print(i)
   */

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Identifier>("i"));

  program.append<AST::ForStatement>(
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("i"),
          make_unique<AST::Literal>(1)),
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("i"),
          AST::BinaryExpression::Op::LessThanOrEquals,
          make_unique<AST::Literal>(10)),
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("i"),
          make_unique<AST::BinaryExpression>(
              make_unique<AST::Identifier>("i"),
              AST::BinaryExpression::Op::Add,
              make_unique<AST::Literal>(1))),
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("print"),
          std::move(args)));

  program.dump();
  interpreter.run(program);
}
