#pragma once
#include "../AST.h"
#include "Lexer.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
namespace Ore::Parser {
class RDParser {
  public:
  explicit RDParser(Ore::Parser::Lexer&);
  std::unique_ptr<AST::Program> parse();

  private:
  Ore::Parser::Lexer& Scanner;
  Ore::Parser::Token Current;
  Ore::Parser::Token Previous;
  bool ErrorFound = false;
  // Helper Functions
  Ore::Parser::Token Peek() { return Current; }
  bool isAtEnd() { return Peek().type() == Ore::Parser::Token::TokenType::Eof; }
  Ore::Parser::Token Advance()
  {
    if (!isAtEnd()) {
      Previous = Current;
      Current = Scanner.next();
    }
    return Previous;
  }
  bool Check(Ore::Parser::Token::TokenType type)
  {
    if (isAtEnd())
      return false;
    else
      return Peek().type() == type;
  }
  template<Ore::Parser::Token::TokenType type, Ore::Parser::Token::TokenType... remained_types>
  bool AdvanceIfMatchAny();
  Ore::Parser::Token ConsumeToken(Ore::Parser::Token::TokenType type, std::string const& ErrorMessage);

  // Parsing Functions
  std::unique_ptr<AST::Statement> ParseStatement();
  std::unique_ptr<AST::Statement> ParseIfStatement();
  std::unique_ptr<AST::Statement> ParseForStatement();
  std::unique_ptr<AST::Statement> ParseWhileStatement();
  std::unique_ptr<AST::Statement> ParseDoWhileStatement();
  std::unique_ptr<AST::Statement> ParseReturnStatement();
  std::unique_ptr<AST::Statement> ParseExportStatement();
  std::unique_ptr<AST::Statement> ParseBreakStatement();
  std::unique_ptr<AST::Statement> ParseContinueStatement();
  std::unique_ptr<AST::Statement> ParseExceptionStatement();
  std::unique_ptr<AST::BlockStatement> ParseBlockStatement();
  std::unique_ptr<AST::Expression> ParseFunction();
  std::vector<AST::FunctionDeclaration::Parameter> ConsumeParameters();
  std::vector<std::unique_ptr<AST::Expression>> ConsumeArguments();
  // Parsing Expressions
  std::unique_ptr<AST::Expression> ParseExpression();
  std::unique_ptr<AST::Expression> OrExpression();
  std::unique_ptr<AST::Expression> XorExpression();
  std::unique_ptr<AST::Expression> AndExpression();
  std::unique_ptr<AST::Expression> EqExpression();
  std::unique_ptr<AST::Expression> CmpExpression();
  std::unique_ptr<AST::Expression> ShfExpression();
  std::unique_ptr<AST::Expression> AddExpression();
  std::unique_ptr<AST::Expression> MulExpression();
  std::unique_ptr<AST::Expression> Unary();
  std::unique_ptr<AST::Expression> Primitive();
  std::vector<std::unique_ptr<AST::Expression>> ConsumeElements();
  std::unique_ptr<AST::Expression> Call();
  double convertToDouble(std::string const& str);
};
}
