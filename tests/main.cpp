#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  std::map<std::string, std::unique_ptr<AST::Expression>> props;
  props["i"] = make_unique<AST::Literal>(5);

  program.append<AST::AssignmentExpression>(
      make_unique<AST::Identifier>("obj"),
      AST::AssignmentExpression::Op::Assignment,
      make_unique<AST::ObjectExpression>(std::move(props)));

  program.append<AST::AssignmentExpression>(
      make_unique<AST::MemberExpression>(
          make_unique<AST::Identifier>("obj"),
          make_unique<AST::Identifier>("i")),
      AST::AssignmentExpression::Op::MultAssignment,
      make_unique<AST::Literal>(5));

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::MemberExpression>(
      make_unique<AST::Identifier>("obj"),
      make_unique<AST::Identifier>("i")));

  program.append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  program.dump();
  interpreter.run(program);
}
