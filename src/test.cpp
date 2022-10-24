#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Value.h"

using std::make_unique;
using namespace Ore;

int main(void)
{

  Interpreter interpreter;
  AST::Program program;

  auto body = make_unique<AST::BlockStatement>();
  body->append<AST::ReturnStatement>(
      make_unique<AST::BinaryExpression>(
          make_unique<AST::Identifier>("bar"),
          AST::BinaryExpression::Op::Sub,
          make_unique<AST::Identifier>("baz")));

  program.append<AST::FunctionDeclaration>(
      "foo",
      std::move(body),
      std::vector<std::string>({ "bar", "baz" }));

  program.append<AST::ReturnStatement>(
      make_unique<AST::CallExpression>(
          make_unique<AST::Identifier>("foo"),
          std::vector<std::shared_ptr<AST::Expression>>({ std::make_shared<AST::Literal>(5), std::make_shared<AST::Literal>(3) })));

  program.dump();

  // calling program as a function allows us to examine the return value.
  auto ret = interpreter.run(program, Interpreter::ScopeType::Function);
  std::cout << ret << std::endl;
}

