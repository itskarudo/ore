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

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::vector<std::shared_ptr<AST::Expression>>({ std::make_shared<AST::Literal>(ore_string(interpreter.heap(), "hello")),
          std::make_shared<AST::Literal>(ore_string(interpreter.heap(), "world")) }));

  program.dump();

  interpreter.run(program);
}

