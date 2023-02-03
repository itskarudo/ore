#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Interpreter interpreter;
  AST::Program program;

  auto try_block = make_unique<AST::BlockStatement>();
  try_block->append<AST::Identifier>("foo");

  std::vector<std::unique_ptr<AST::Expression>> args;
  args.push_back(make_unique<AST::StringLiteral>("GOT AN ERROR"));
  args.push_back(make_unique<AST::MemberExpression>(
      make_unique<AST::Identifier>("err"),
      make_unique<AST::Identifier>("message")));

  auto catch_block = make_unique<AST::BlockStatement>();
  catch_block->append<AST::CallExpression>(
      make_unique<AST::Identifier>("print"),
      std::move(args));

  auto catch_clause = make_unique<AST::CatchClause>("err", std::move(catch_block));

  program.append<AST::TryStatement>(
      std::move(try_block),
      std::move(catch_clause), std::nullopt);

  program.dump();
  interpreter.run(program);
  if (interpreter.has_exception()) {
    std::cout << "Uncaught Exception: " << interpreter.exception()->message() << std::endl;
  }
}
