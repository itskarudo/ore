#pragma once
#include <string>

namespace Ore::Parser {

#define ENUMERATE_TOKEN_TYPES                         \
  __ENUM_TOKEN_TYPES(If, Keyword)                     \
  __ENUM_TOKEN_TYPES(Else, Keyword)                   \
  __ENUM_TOKEN_TYPES(Function, Keyword)               \
  __ENUM_TOKEN_TYPES(While, Keyword)                  \
  __ENUM_TOKEN_TYPES(For, Keyword)                    \
  __ENUM_TOKEN_TYPES(Do, Keyword)                     \
  __ENUM_TOKEN_TYPES(Break, Keyword)                  \
  __ENUM_TOKEN_TYPES(Continue, Keyword)               \
  __ENUM_TOKEN_TYPES(Switch, Keyword)                 \
  __ENUM_TOKEN_TYPES(Case, Keyword)                   \
  __ENUM_TOKEN_TYPES(Default, Keyword)                \
  __ENUM_TOKEN_TYPES(Try, Keyword)                    \
  __ENUM_TOKEN_TYPES(Catch, Keyword)                  \
  __ENUM_TOKEN_TYPES(Finally, Keyword)                \
  __ENUM_TOKEN_TYPES(Global, Keyword)                 \
  __ENUM_TOKEN_TYPES(Return, Keyword)                 \
  __ENUM_TOKEN_TYPES(Export, Keyword)                 \
  __ENUM_TOKEN_TYPES(BoolLiteral, Keyword)            \
  __ENUM_TOKEN_TYPES(NilLiteral, Keyword)             \
  __ENUM_TOKEN_TYPES(CurlyOpen, Punctuation)          \
  __ENUM_TOKEN_TYPES(CurlyClose, Punctuation)         \
  __ENUM_TOKEN_TYPES(BracketOpen, Punctuation)        \
  __ENUM_TOKEN_TYPES(BracketClose, Punctuation)       \
  __ENUM_TOKEN_TYPES(ParenOpen, Punctuation)          \
  __ENUM_TOKEN_TYPES(ParenClose, Punctuation)         \
  __ENUM_TOKEN_TYPES(Equals, Operator)                \
  __ENUM_TOKEN_TYPES(PlusEquals, Operator)            \
  __ENUM_TOKEN_TYPES(MinusEquals, Operator)           \
  __ENUM_TOKEN_TYPES(AsteriskEquals, Operator)        \
  __ENUM_TOKEN_TYPES(DoubleAsterisk, Operator)        \
  __ENUM_TOKEN_TYPES(SlashEquals, Operator)           \
  __ENUM_TOKEN_TYPES(ShiftLeftEquals, Operator)       \
  __ENUM_TOKEN_TYPES(ShiftRightEquals, Operator)      \
  __ENUM_TOKEN_TYPES(DoubleDotEquals, Operator)       \
  __ENUM_TOKEN_TYPES(Percent, Operator)               \
  __ENUM_TOKEN_TYPES(Hash, Operator)                  \
  __ENUM_TOKEN_TYPES(Plus, Operator)                  \
  __ENUM_TOKEN_TYPES(Minus, Operator)                 \
  __ENUM_TOKEN_TYPES(Asterisk, Operator)              \
  __ENUM_TOKEN_TYPES(Slash, Operator)                 \
  __ENUM_TOKEN_TYPES(EqualsEquals, Operator)          \
  __ENUM_TOKEN_TYPES(ExclamationMarkEquals, Operator) \
  __ENUM_TOKEN_TYPES(LargerThan, Operator)            \
  __ENUM_TOKEN_TYPES(LessThan, Operator)              \
  __ENUM_TOKEN_TYPES(LargerThanEquals, Operator)      \
  __ENUM_TOKEN_TYPES(LessThanEquals, Operator)        \
  __ENUM_TOKEN_TYPES(ShiftLeft, Operator)             \
  __ENUM_TOKEN_TYPES(ShiftRight, Operator)            \
  __ENUM_TOKEN_TYPES(DoubleDot, Operator)             \
  __ENUM_TOKEN_TYPES(And, Operator)                   \
  __ENUM_TOKEN_TYPES(Or, Operator)                    \
  __ENUM_TOKEN_TYPES(Xor, Operator)                   \
  __ENUM_TOKEN_TYPES(Not, Operator)                   \
  __ENUM_TOKEN_TYPES(TripleDot, Operator)             \
  __ENUM_TOKEN_TYPES(Dot, Operator)                   \
  __ENUM_TOKEN_TYPES(Colon, Punctuation)              \
  __ENUM_TOKEN_TYPES(Comma, Punctuation)              \
  __ENUM_TOKEN_TYPES(Eof, Invalid)                    \
  __ENUM_TOKEN_TYPES(Invalid, Invalid)                \
  __ENUM_TOKEN_TYPES(NumberLiteral, Number)           \
  __ENUM_TOKEN_TYPES(StringLiteral, String)           \
  __ENUM_TOKEN_TYPES(SemiColon, Punctuation)          \
  __ENUM_TOKEN_TYPES(Identifier, Identifier)

class Token {
  public:
  enum class TokenType {
#define __ENUM_TOKEN_TYPES(type, category) type,
    ENUMERATE_TOKEN_TYPES
#undef __ENUM_TOKEN_TYPES
  };

  Token() = default;

  Token(TokenType type, std::string_view value, size_t line_number, size_t line_column)
      : m_type(type)
      , m_value(value)
      , m_line_number(line_number)
      , m_line_column(line_column)
  {
  }

  TokenType type() const { return m_type; }
  char const* name() const;
  std::string const& value() const { return m_value; }
  size_t line_number() const { return m_line_number; }
  size_t line_column() const { return m_line_column; }

  void set_value(std::string const& value) { m_value = value; }

  void dump() const;

  private:
  TokenType m_type { TokenType::Invalid };
  std::string m_value;
  size_t m_line_number { 0 };
  size_t m_line_column { 0 };
};

}
