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

  program.append<AST::ExportStatement>(
      make_unique<AST::AssignmentExpression>(
          make_unique<AST::Identifier>("foo"),
          make_unique<AST::Literal>(69)));

  program.dump();

  interpreter.run(program);
}

