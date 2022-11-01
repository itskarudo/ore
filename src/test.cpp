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

  auto foo_body = make_unique<AST::BlockStatement>();

  std::vector<std::unique_ptr<AST::Expression>> print_args;
  print_args.push_back(make_unique<AST::Identifier>("str"));
  print_args.push_back(make_unique<AST::Identifier>("num"));

  foo_body->append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(print_args));

  std::vector<AST::FunctionDeclaration::Parameter> foo_params;
  foo_params.push_back({ "str", std::nullopt });
  foo_params.push_back({ "num", make_unique<AST::Literal>(69) });

  program.append<AST::FunctionDeclaration>(
      "foo",
      std::move(foo_body),
      std::move(foo_params));

  std::vector<std::unique_ptr<AST::Expression>> foo_args;
  foo_args.push_back(make_unique<AST::Literal>(ore_string(interpreter.heap(), "Aloha, universe!")));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("foo"),
      std::move(foo_args));

  program.dump();

  interpreter.run(program);
}

