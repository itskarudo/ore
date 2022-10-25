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

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::Literal>(
      ore_string(interpreter.heap(), "bonjour world")));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();

  interpreter.run(program);
}

