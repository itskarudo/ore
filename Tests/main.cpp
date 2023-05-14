#include <Ore.h>
using std::make_unique;

int main(void)
{

	Ore::Interpreter interpreter;
	std::string_view str = R"(
		var = 1 +2*3
	)";
	std::cout << str << std::endl;
	Ore::Parser::Lexer TestLexer(str);
	Ore::Parser::RDParser TestParser(TestLexer);
	auto program = TestParser.Parse();
	program->dump();
	//auto retval = interpreter.run(*(program.get()));
}
