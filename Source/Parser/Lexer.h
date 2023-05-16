#pragma once
#include "Token.h"
#include <map>
#include <stdio.h>
#include <string>
#include <string_view>

namespace Ore::Parser {

class Lexer {
  public:
  explicit Lexer(std::string_view source);
  Token next();

  std::string_view& source() { return m_source; }

  private:
  void consume();

  inline bool is_line_terminator();
  inline bool is_whitespace();
  inline bool is_comment_start();
  inline bool is_identifier_start();
  inline bool is_identifier_middle();
  inline bool isoctal(char c);
  bool match(char, char) const;
  bool match(char, char, char) const;

  size_t m_line_number { 1 };
  size_t m_line_column { 0 };
  std::string_view m_source;
  size_t m_position { 0 };
  char m_current_char;
  Token m_current_token;
  bool m_eof { false };
  Token::TokenType m_token_type { Token::TokenType::Invalid };

  static std::map<std::string_view, Token::TokenType> s_keywords;
  static std::map<std::string_view, Token::TokenType> s_three_char_tokens;
  static std::map<std::string_view, Token::TokenType> s_two_char_tokens;
  static std::map<char, Token::TokenType> s_one_char_tokens;
};

}
