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

  elements.push_back(make_unique<AST::Literal>(
      ore_string(interpreter.heap(), "Good Morning USA")));

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("bar"),
      make_unique<AST::ArrayExpression>(std::move(elements)));

  std::vector<std::unique_ptr<AST::Expression>> params;

  params.push_back(make_unique<AST::MemberExpression>(
      make_unique<AST::Identifier>("bar"),
      make_unique<AST::Literal>(0), true));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(params));

  program.dump();

  interpreter.run(program);
}

