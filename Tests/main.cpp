#include <Ore.h>

int main(void)
{

  std::string_view str = R"(
		var = 1 +2*3
	)";
  std::cout << str << std::endl;
  Ore::Parser::Lexer TestLexer(str);
  Ore::Parser::RDParser TestParser(TestLexer);
  auto program = TestParser.parse();
  program->dump();
}
