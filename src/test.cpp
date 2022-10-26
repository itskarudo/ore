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

  std::vector<std::unique_ptr<AST::Expression>> elements;

  elements.push_back(make_unique<AST::Literal>(5));
  elements.push_back(make_unique<AST::CallExpression>(
      make_unique<AST::Identifier>("print")));

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("bar"),
      make_unique<AST::ArrayExpression>(std::move(elements)));

  program.dump();

  interpreter.run(program);
}

