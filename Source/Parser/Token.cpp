#include "Token.h"
#include <iostream>

namespace Ore::Parser {
char const* Token::name() const
{
  switch (m_type) {
#define __ENUM_TOKEN_TYPES(type, category) \
  case TokenType::type:                    \
    return #type;

    ENUMERATE_TOKEN_TYPES
#undef __ENUM_TOKEN_TYPES
  default:
    return "Invalid";
  }
}

void Token::dump() const
{
  std::cout << "\033[34m# Token \033[33m(" << name() << ")\033[0m" << std::endl;

  if (type() == TokenType::Identifier || type() == TokenType::NumberLiteral || type() == TokenType::BoolLiteral || type() == TokenType::StringLiteral)
    std::cout << "  \033[31m" << value() << "\033[0m" << std::endl;
}

}
