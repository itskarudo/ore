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

  std::map<std::string, std::unique_ptr<AST::Expression>> a;
  a["bar"] = make_unique<AST::Literal>(5);
  a["baz"] = make_unique<AST::ObjectExpression>();

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("foo"),
      make_unique<AST::ObjectExpression>(
          std::move(a)));

  program.dump();

  interpreter.run(program);
}

