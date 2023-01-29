#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::map<std::string, std::unique_ptr<AST::Expression>> object;
  object["foo"] = make_unique<AST::NumberLiteral>(5);
  object["bar"] = make_unique<AST::NilLiteral>();
  object["baz"] = make_unique<AST::StringLiteral>("hi");

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::ObjectExpression>(std::move(object)));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);
}
