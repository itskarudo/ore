#include <Ore.h>

using namespace Ore;
using std::make_unique;

int main(void)
{
  Parser::Lexer l("1.2, 0x5FB, 0o245, 0b107110");
  Parser::Token t;

  do {
    t = l.next();
    std::cout << "Token type: " << t.name() << ", value: " << t.value() << std::endl;
  } while (t.type() != Parser::Token::TokenType::Eof);
}
