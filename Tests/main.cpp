#include <Parser/Lexer.h>
#include <iostream>

using namespace Ore;

int main(void)
{
  auto lx = Parser::Lexer("print('helloooo') -- this is a comment");
  Parser::Token t = lx.next();
  while (t.type() != Parser::Token::TokenType::Eof) {
    std::cout << "type: " << t.name() << ", value: " << t.value() << std::endl;
    t = lx.next();
  }
}
