#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  auto tr = make_unique<AST::BlockStatement>();
  tr->append<AST::ReturnStatement>(
      make_unique<AST::Literal>(true));

  auto body = make_unique<AST::BlockStatement>();
  body->append<AST::IfStatement>(
      make_unique<AST::Literal>(true),
      std::move(tr),
      make_unique<AST::BlockStatement>());

  program.append<AST::FunctionDeclaration>(
      "foo",
      std::move(body));

  program.append<AST::IfStatement>(
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("foo")),
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("DEBUG")),
      make_unique<AST::BlockStatement>());

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

