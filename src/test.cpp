#include "AST.h"
#include "Interpreter.h"
#include "Runtime/Object.h"

using std::make_unique;
using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  auto body = std::make_shared<AST::ScopeNode>();

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("foo"),
      make_unique<AST::FunctionDeclaration>(
          std::nullopt, body));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("foo"));

  program.dump();

  auto ret = interpreter.run(program);
  std::cout << ret << std::endl;
}

