#include "AST.h"
#include "Interpreter.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("counter"),
      make_unique<AST::Literal>(0));

  auto while_body = make_unique<AST::ScopeNode>();
  while_body->append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("counter"),
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("counter"),
          AST::BinaryExpression::Op::Add,
          make_unique<AST::Literal>(1)));

  program.append<AST::WhileStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("counter"),
          AST::BinaryExpression::Op::LessThanOrEquals,
          make_unique<AST::Literal>(5)),
      std::move(while_body));

  program.append<AST::ReturnStatement>(
      make_unique<AST::Identifier>("counter"));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

