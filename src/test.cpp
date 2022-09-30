#include "AST.h"
#include "Interpreter.h"

using namespace Ore;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  auto b = std::make_unique<AST::ScopeNode>();
  b->append<AST::VariableDeclaration>(
      std::make_unique<AST::Identifier>("bar"),
      std::make_unique<AST::Literal>(ore_string(interpreter.heap(), "baz")));

  program.append<AST::FunctionDeclaration>("foo", std::move(b));
  program.append<AST::CallExpression>("foo");

  program.dump();

  auto ret = interpreter.run(program);
}

