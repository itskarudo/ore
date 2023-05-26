#include "Lexer.h"
#include <ctype.h>
#include <iostream>
#include <sstream>

namespace Ore::Parser {

std::map<std::string_view, Token::TokenType> Lexer::s_keywords;
std::map<std::string_view, Token::TokenType> Lexer::s_three_char_tokens;
std::map<std::string_view, Token::TokenType> Lexer::s_two_char_tokens;
std::map<char, Token::TokenType> Lexer::s_one_char_tokens;

Lexer::Lexer(std::string_view source)
    : m_source(source)
{
  if (s_keywords.empty()) {
    s_keywords["if"] = Token::TokenType::If;
    s_keywords["else"] = Token::TokenType::Else;
    s_keywords["function"] = Token::TokenType::Function;
    s_keywords["while"] = Token::TokenType::While;
    s_keywords["for"] = Token::TokenType::For;
    s_keywords["do"] = Token::TokenType::Do;
    s_keywords["break"] = Token::TokenType::Break;
    s_keywords["continue"] = Token::TokenType::Continue;
    s_keywords["switch"] = Token::TokenType::Switch;
    s_keywords["case"] = Token::TokenType::Case;
    s_keywords["default"] = Token::TokenType::Default;
    s_keywords["try"] = Token::TokenType::Try;
    s_keywords["catch"] = Token::TokenType::Catch;
    s_keywords["finally"] = Token::TokenType::Finally;
    s_keywords["global"] = Token::TokenType::Global;
    s_keywords["return"] = Token::TokenType::Return;
    s_keywords["export"] = Token::TokenType::Export;
    s_keywords["true"] = Token::TokenType::BoolLiteral;
    s_keywords["false"] = Token::TokenType::BoolLiteral;
    s_keywords["nil"] = Token::TokenType::NilLiteral;
    s_keywords["and"] = Token::TokenType::And;
    s_keywords["xor"] = Token::TokenType::Xor;
    s_keywords["or"] = Token::TokenType::Or;
    s_keywords["not"] = Token::TokenType::Not;
  }

  if (s_three_char_tokens.empty()) {
    s_three_char_tokens["<<="] = Token::TokenType::ShiftLeftEquals;
    s_three_char_tokens[">>="] = Token::TokenType::ShiftRightEquals;
    s_three_char_tokens["..="] = Token::TokenType::DoubleDotEquals;
    s_three_char_tokens["..."] = Token::TokenType::TripleDot;
  }

  if (s_two_char_tokens.empty()) {
    s_two_char_tokens["+="] = Token::TokenType::PlusEquals;
    s_two_char_tokens["-="] = Token::TokenType::MinusEquals;
    s_two_char_tokens["*="] = Token::TokenType::AsteriskEquals;
    s_two_char_tokens["/="] = Token::TokenType::SlashEquals;
    s_two_char_tokens["!="] = Token::TokenType::ExclamationMarkEquals;
    s_two_char_tokens["=="] = Token::TokenType::EqualsEquals;
    s_two_char_tokens["<="] = Token::TokenType::LessThanEquals;
    s_two_char_tokens[">="] = Token::TokenType::LargerThanEquals;
    s_two_char_tokens[".."] = Token::TokenType::DoubleDot;
    s_two_char_tokens["<<"] = Token::TokenType::ShiftLeft;
    s_two_char_tokens[">>"] = Token::TokenType::ShiftRight;
  }

  if (s_one_char_tokens.empty()) {
    s_one_char_tokens['{'] = Token::TokenType::CurlyOpen;
    s_one_char_tokens['}'] = Token::TokenType::CurlyClose;
    s_one_char_tokens['['] = Token::TokenType::BracketOpen;
    s_one_char_tokens[']'] = Token::TokenType::BracketClose;
    s_one_char_tokens['('] = Token::TokenType::ParenOpen;
    s_one_char_tokens[')'] = Token::TokenType::ParenClose;
    s_one_char_tokens['='] = Token::TokenType::Equals;
    s_one_char_tokens['#'] = Token::TokenType::Hash;
    s_one_char_tokens['+'] = Token::TokenType::Plus;
    s_one_char_tokens['-'] = Token::TokenType::Minus;
    s_one_char_tokens['*'] = Token::TokenType::Asterisk;
    s_one_char_tokens['/'] = Token::TokenType::Slash;
    s_one_char_tokens['.'] = Token::TokenType::Dot;
    s_one_char_tokens[','] = Token::TokenType::Comma;
    s_one_char_tokens[':'] = Token::TokenType::Colon;
    s_one_char_tokens['<'] = Token::TokenType::LessThan;
    s_one_char_tokens['>'] = Token::TokenType::LargerThan;
    s_one_char_tokens[';'] = Token::TokenType::SemiColon;
    s_one_char_tokens['%'] = Token::TokenType::Percent;
  }

  consume();
}

void Lexer::consume()
{
  if (m_position >= m_source.length()) {
    m_current_char = '\0';
    m_position++;
    m_eof = true;
    return;
  }

  m_current_char = m_source[m_position++];
  m_line_column++;
}

Token Lexer::next()
{
  while (true) {
    if (is_line_terminator()) {
      do {
        consume();
        m_line_number++;
        m_line_column = 1;
      } while (!m_eof && is_line_terminator());
    } else if (is_whitespace()) {
      do {
        consume();
      } while (!m_eof && is_whitespace());
    } else if (is_comment_start()) {
      do {
        consume();
      } while (!m_eof && !is_line_terminator());
    } else
      break;
  }

  std::string identifier;
  size_t value_start = m_position;
  size_t saved_column = m_line_column;

  if (is_identifier_start()) {
    std::stringstream s;
    do {
      s << m_current_char;
      consume();
    } while (is_identifier_middle());

    identifier = s.str();

    if (s_keywords.contains(identifier))
      m_token_type = s_keywords.at(identifier);
    else
      m_token_type = Token::TokenType::Identifier;

  } else if (isdigit(m_current_char)) {
    m_token_type = Token::TokenType::NumberLiteral;
    if (m_current_char == '0') {
      consume();
      if (isdigit(m_current_char) || m_current_char == '.') {
        while (isdigit(m_current_char))
          consume();

        if (m_current_char == '.') {
          do {
            consume();
          } while (isdigit(m_current_char));
        }
      } else if (m_current_char == 'x') {
        do {
          consume();
        } while (isxdigit(m_current_char));
      } else if (m_current_char == 'o') {
        do {
          consume();
        } while (isoctal(m_current_char));
      } else if (m_current_char == 'b') {
        do {
          consume();
        } while (m_current_char == '0' || m_current_char == '1');
      }
    } else {
      do {
        consume();
      } while (isdigit(m_current_char));

      if (m_current_char == '.') {
        do {
          consume();
        } while (isdigit(m_current_char));
      }
    }

  } else if (m_eof) {
    m_token_type = Token::TokenType::Eof;

  } else if (m_current_char == '\'' || m_current_char == '"') {

    char end_char = m_current_char;
    consume();
    while (m_current_char != end_char && !m_eof)
      consume();

    if (m_current_char != end_char)
      m_token_type = Token::TokenType::Invalid;
    else {
      consume();
      m_token_type = Token::TokenType::StringLiteral;
    }

  } else {

    bool found_three_char_token = false;

    if (m_position + 1 < m_source.length()) {
      auto value = m_source.substr(m_position - 1, 3);
      if (s_three_char_tokens.contains(value)) {
        found_three_char_token = true;
        m_token_type = s_three_char_tokens.at(value);
        consume();
        consume();
        consume();
      }
    }

    bool found_two_char_token = false;

    if (!found_three_char_token && m_position < m_source.length()) {
      auto value = m_source.substr(m_position - 1, 2);
      if (s_two_char_tokens.contains(value)) {
        found_two_char_token = true;
        m_token_type = s_two_char_tokens.at(value);
        consume();
        consume();
      }
    }

    bool found_one_char_token = false;

    if (!found_three_char_token && !found_two_char_token) {
      if (s_one_char_tokens.contains(m_current_char)) {
        found_one_char_token = true;
        m_token_type = s_one_char_tokens.at(m_current_char);
        consume();
      }
    }

    if (!found_three_char_token && !found_two_char_token && !found_one_char_token) {
      consume();
      m_token_type = Token::TokenType::Invalid;
    }
  }

  m_current_token = Token(m_token_type, m_source.substr(value_start - 1, m_position - value_start), m_line_number, saved_column);
  if (!identifier.empty())
    m_current_token.set_value(identifier);
  return m_current_token;
}

inline bool Lexer::is_line_terminator()
{
  return m_current_char == '\n' || m_current_char == '\r';
}

inline bool Lexer::is_whitespace()
{
  return isspace(m_current_char);
}

inline bool Lexer::is_comment_start()
{
  return match('-', '-');
}

inline bool Lexer::is_identifier_start()
{
  return isalpha(m_current_char) || (m_current_char == '_');
}

inline bool Lexer::is_identifier_middle()
{
  return isalnum(m_current_char) || (m_current_char == '_');
}

inline bool Lexer::isoctal(char c)
{
  return c >= '0' && c <= '7';
}

bool Lexer::match(char a, char b) const
{
  if (m_position >= m_source.length())
    return false;

  return m_current_char == a && m_source[m_position] == b;
}

bool Lexer::match(char a, char b, char c) const
{
  if (m_position + 1 >= m_source.length())
    return false;

  return m_current_char == a && m_source[m_position] == b && m_source[m_position + 1] == c;
}

}
