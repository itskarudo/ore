#include "Token.h"

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
}
