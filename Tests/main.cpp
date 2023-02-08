#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("a"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::ObjectExpression>());

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("b"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::ObjectExpression>());

  program.append<AST::AssignmentExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("a"),
          make_unique<AST::Identifier>("v")),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::Identifier>("b"));

  program.append<AST::AssignmentExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("b"),
          make_unique<AST::Identifier>("u")),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::Identifier>("a"));

  program.dump();
  interpreter.run(program);
}
