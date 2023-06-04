#include "RDParser.h"
#include "Token.h"
#include <fmt/core.h>
#include <iterator>
#include <memory>
/*
code ::= {Statement};
Statement ::=
              IfStatement |------------------------------------------------
              WhileStatement |---------------------------------------------
              DoWhileStatement |-------------------------------------------
              ForStatement |-----------------------------------------------
              ReturnStatement |--------------------------------------------
              ExportStatement |--------------------------------------------
              ExceptionStatement|------------------------------------------
              AssignmentStatement|
              BreakStatement|----------------------------------------------
              ContinueStatement|-------------------------------------------
              BlockStatement |---------------------------------------------
              Expression
IfStatement ::= "if" Expression Statement
WhileStatement ::= "while" Expression Statement
ForStatement ::= "for" IDENTIFIER "in" IDENTIFIER Statement
ReturnStatement ::= "return" (expression)
ExportStatement ::= "export" Expression
ExceptionStatement ::= "try" "{" Statement "}" "catch" "{" Statement "}"
AssignStatement ::= [global] IDENTIFER assign Expression
BlockStatement ::= "{" code "}"
VariableDeclaration ::= [global] IdentifierList assign ExpressionList
IdentifierList ::= IDENTIFIER {, IDENTIFIER}
ExpressionList ::= Expression {, Expression}
FunctionDeclaration ::= "function" IDENTIFIER FunctionBody
Expression ::= literal | unary | binary | grouping | FunctionCall | Anonymous | ArrayConstrutor | MapConstructor | IDENTIFIER | FunctionDeclaration | MemberCall
literal ::= NUMBER | STRING | "true" | "false" | "nil"
grouping ::= "(" Expression ")"
unary ::= ("#" | "-" | "!" | "~") Expression
binary ::= Expression Operator Expression
Operator ::= "+" | "-" | "*" | "/" | "%" | "**" | "and" | "or" | "xor" | "&" | "|" | "^" | "<<" | ">>" | ".." | "<" | ">" | "<=" | ">=" | "!=" | "=="
assign ::=  ("+" | "-" | "*" | "/" | "%" | "**" | "&" | "|" | "^" | "<<" | ">>" | "..") "="
Anonymous ::= "function" FunctionBody
ArrayConstructor ::= "[" ExpressionList "]"
MapContructor ::= "{" Member { "," Member} "}"
Member ::= IDENTIFIER ":" Expression
FunctionCall ::= IDENTIFIER "(" ExpressionList ")"
FunctionBody ::= "(" Args ")" Statement
Args ::=  [IdentifierList]
MemberCall ::= IDENTIFIER"["Expression"]" | IDENTIFIER"."IDENTIFIER
*/
namespace Ore::Parser {
RDParser::RDParser(Ore::Parser::Lexer& Scanner, bool dump_tokens)
    : Scanner(Scanner)
    , m_dump_tokens(dump_tokens)
{
  this->Current = this->Scanner.next();
  this->Previous = Ore::Parser::Token {};

  if (dump_tokens)
    this->Current.dump();
}
template<Ore::Parser::Token::TokenType type, Ore::Parser::Token::TokenType... remained_types>
bool RDParser::AdvanceIfMatchAny()
{
  if (!Check(type)) {
    if constexpr (sizeof...(remained_types) > 0) {
      return AdvanceIfMatchAny<remained_types...>();
    } else {
      return false;
    }
  }
  Advance();
  return true;
}
Ore::Parser::Token RDParser::ConsumeToken(Ore::Parser::Token::TokenType type, std::string const& ErrorMessage)
{
  if (Check(type)) {
    return Advance();
  }
  m_error_message = ErrorMessage;
  ErrorFound = true;
  return Ore::Parser::Token {};
}
std::unique_ptr<AST::Program> RDParser::parse()
{
  auto Program = std::make_unique<AST::Program>(SourceRange {});
  while (!isAtEnd()) {
    auto Statement = ParseStatement();
    if (!ErrorFound)
      Program->append(std::move(Statement));
    else
      return std::make_unique<AST::Program>(SourceRange {});
  }
  return std::move(Program);
}
std::unique_ptr<AST::Statement> RDParser::ParseStatement()
{
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::If>()) {
    return ParseIfStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Else>()) {
    m_error_message = "Else must be preceded by an if block.";
    ErrorFound = true;
    return std::make_unique<AST::BlockStatement>(SourceRange {});
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::For>()) {
    return ParseForStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::While>()) {
    return ParseWhileStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Do>()) {
    return ParseDoWhileStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Break>()) {
    return ParseBreakStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Continue>()) {
    return ParseContinueStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Return>()) {
    return ParseReturnStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Export>()) {
    return ParseExportStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Try>()) {
    return ParseExceptionStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Catch>()) {
    m_error_message = "Catch must be preceded by a try block.";
    ErrorFound = true;
    return std::make_unique<AST::BlockStatement>(SourceRange {});
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyOpen>()) {
    return ParseBlockStatement();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyClose>()) {
    m_error_message = "} must be preceded by a block.";
    ErrorFound = true;
    return std::make_unique<AST::BlockStatement>(SourceRange {});
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Switch, Ore::Parser::Token::TokenType::Case>()) {
    m_error_message = "Switch Case is not yet emplimented.";
    ErrorFound = true;
    return std::make_unique<AST::BlockStatement>(SourceRange {});
  } else
    return ParseExpression();
}
std::unique_ptr<AST::Statement> RDParser::ParseIfStatement()
{
  auto Test = ParseExpression();
  auto Consequent = ParseStatement();
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Else>()) {
    auto Alternate = ParseStatement();
    return std::make_unique<AST::IfStatement>(SourceRange {}, std::move(Test), std::move(Consequent), std::move(Alternate));
  }
  return std::make_unique<AST::IfStatement>(SourceRange {}, std::move(Test), std::move(Consequent), std::nullopt);
}
std::unique_ptr<AST::Statement> RDParser::ParseForStatement()
{
  ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected ( after for taken ");
  if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::SemiColon>()) {
    auto Init = ParseExpression();
    ConsumeToken(Ore::Parser::Token::TokenType::SemiColon, "expected ; after initilizer");
    if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::SemiColon>()) {
      auto Test = ParseExpression();
      ConsumeToken(Ore::Parser::Token::TokenType::SemiColon, "expected ; after Tester");
      if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
        auto Update = ParseExpression();
        ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "expected ) after for ");
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::move(Init), std::move(Test), std::move(Update), std::move(Body));
      } else {
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::move(Init), std::move(Test), std::nullopt, std::move(Body));
      }
    } else {
      if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
        auto Update = ParseExpression();
        ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "expected ) after for ");
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::move(Init), std::nullopt, std::move(Update), std::move(Body));
      } else {
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::move(Init), std::nullopt, std::nullopt, std::move(Body));
      }
    }
  } else {
    if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::SemiColon>()) {
      auto Test = ParseExpression();
      ConsumeToken(Ore::Parser::Token::TokenType::SemiColon, "expected ; after Tester");
      if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
        auto Update = ParseExpression();
        ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "expected ) after for ");
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::nullopt, std::move(Test), std::move(Update), std::move(Body));
      } else {
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::nullopt, std::move(Test), std::nullopt, std::move(Body));
      }
    } else {
      if (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
        auto Update = ParseExpression();
        ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "expected ) after for ");
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::nullopt, std::nullopt, std::move(Update), std::move(Body));
      } else {
        auto Body = ParseStatement();
        return std::make_unique<AST::ForStatement>(SourceRange {}, std::nullopt, std::nullopt, std::nullopt, std::move(Body));
      }
    }
  }
}
std::unique_ptr<AST::Statement> RDParser::ParseWhileStatement()
{
  auto Test = ParseExpression();
  auto Body = ParseStatement();
  return std::make_unique<AST::WhileStatement>(SourceRange {}, std::move(Test), std::move(Body));
}
std::unique_ptr<AST::Statement> RDParser::ParseDoWhileStatement()
{
  auto Body = ParseStatement();
  ConsumeToken(Ore::Parser::Token::TokenType::While, "expected while after do block");
  auto Test = ParseExpression();
  return std::make_unique<AST::DoWhileStatement>(SourceRange {}, std::move(Test), std::move(Body));
}
std::unique_ptr<AST::Statement> RDParser::ParseReturnStatement()
{
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
    auto Expr = ParseExpression();
    ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "Expected ) after ( taken");
    return std::make_unique<AST::ReturnStatement>(SourceRange {}, std::move(Expr));
  }
  return std::make_unique<AST::ReturnStatement>(SourceRange {}, std::nullopt);
}
std::unique_ptr<AST::Statement> RDParser::ParseExportStatement()
{
  auto Expr = ParseExpression();
  return std::make_unique<AST::ExportStatement>(SourceRange {}, std::move(Expr));
}
std::unique_ptr<AST::Statement> RDParser::ParseBreakStatement()
{
  return std::make_unique<AST::BreakStatement>(SourceRange {});
}
std::unique_ptr<AST::Statement> RDParser::ParseContinueStatement()
{
  return std::make_unique<AST::ContinueStatement>(SourceRange {});
}
std::unique_ptr<AST::Statement> RDParser::ParseExceptionStatement()
{
  ConsumeToken(Ore::Parser::Token::TokenType::CurlyOpen, "expected a block after try");
  auto Block = ParseBlockStatement();
  ConsumeToken(Ore::Parser::Token::TokenType::Catch, "expected catch after try block");
  ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected ( after catch");
  auto Exp = Current.value();
  ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected name after catch");
  ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "expected ) after variable name");
  ConsumeToken(Ore::Parser::Token::TokenType::CurlyOpen, "expected a block after catch");
  auto HandlerBlock = ParseBlockStatement();
  auto Handler = std::make_unique<AST::CatchClause>(SourceRange {}, Exp, std::move(HandlerBlock));
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Finally>()) {
    ConsumeToken(Ore::Parser::Token::TokenType::CurlyOpen, "expected a block after finally");
    auto Finalizer = ParseBlockStatement();
    return std::make_unique<AST::TryStatement>(SourceRange {}, std::move(Block), std::move(Handler), std::move(Finalizer));
  }
  return std::make_unique<AST::TryStatement>(SourceRange {}, std::move(Block), std::move(Handler), std::nullopt);
}
std::unique_ptr<AST::BlockStatement> RDParser::ParseBlockStatement()
{
  auto Block = std::make_unique<AST::BlockStatement>(SourceRange {});
  while (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyClose>()) {
    auto Statement = ParseStatement();
    Block->append(std::move(Statement));
    if (isAtEnd()) {
      m_error_message = "} expected after a block.";
      ErrorFound = true;
      return std::move(Block);
    }
  }
  return std::move(Block);
}
std::unique_ptr<AST::Expression> RDParser::ParseFunction()
{
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Identifier>()) {
    auto FuncName = Previous.value();
    ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected ( after functon's name ");
    auto ParamVector = std::move(ConsumeParameters());
    ConsumeToken(Ore::Parser::Token::TokenType::CurlyOpen, "function body needs to be a block statement ");
    std::shared_ptr<AST::BlockStatement> Body = std::move(ParseBlockStatement());
    return std::make_unique<AST::FunctionDeclaration>(SourceRange {}, FuncName, Body, std::move(ParamVector));
  }
  ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected ( after functon's name ");
  auto ParamVector = std::move(ConsumeParameters());
  ConsumeToken(Ore::Parser::Token::TokenType::CurlyOpen, "function body needs to be a block statement ");
  std::shared_ptr<AST::BlockStatement> Body = std::move(ParseBlockStatement());
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
    auto ArgVector = std::move(ConsumeArguments());
    return std::make_unique<AST::CallExpression>(SourceRange {}, std::make_unique<AST::FunctionDeclaration>(SourceRange {}, std::nullopt, Body, std::move(ParamVector)), std::move(ArgVector));
  }
  return std::make_unique<AST::FunctionDeclaration>(SourceRange {}, std::nullopt, Body, std::move(ParamVector));
}

std::vector<AST::FunctionDeclaration::Parameter> RDParser::ConsumeParameters()
{
  std::vector<AST::FunctionDeclaration::Parameter> ParamVector;
  std::vector<std::string> VarNames;
  while (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
    auto VarName = Current.value();
    for (std::string nm : VarNames) {
      if (nm == VarName) {
        m_error_message = "Multiple parameters can't have the same name.";
        ErrorFound = true;
      }
    }
    VarNames.push_back(VarName);
    if (Current.type() != Ore::Parser::Token::TokenType::Identifier) {
      m_error_message = "Variable name needs to be a string.";
      ErrorFound = true;
      break;
    }
    ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected variable name ");
    if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Equals>()) {
      auto VarValue = std::move(ParseExpression());
      AST::FunctionDeclaration::Parameter Param { VarName, std::move(VarValue) };
      ParamVector.push_back(std::move(Param));
      if (!Check(Ore::Parser::Token::TokenType::ParenClose)) {
        ConsumeToken(Ore::Parser::Token::TokenType::Comma, "expected comma between arguments");
        if (Check(Ore::Parser::Token::TokenType::ParenClose)) {
          ErrorFound = true;
          m_error_message = "Expected variable name between commas.";
        }
      }
      if (isAtEnd()) {
        ErrorFound = true;
        m_error_message = "expected ) after arguments.";
      }
    } else {
      AST::FunctionDeclaration::Parameter Param { VarName, std::nullopt };
      ParamVector.push_back(std::move(Param));
      if (!Check(Ore::Parser::Token::TokenType::ParenClose)) {
        ConsumeToken(Ore::Parser::Token::TokenType::Comma, "expected comma between arguments");
        if (Check(Ore::Parser::Token::TokenType::ParenClose)) {
          ErrorFound = true;
          m_error_message = "Expected variable name between commas.";
        }
      }
      if (isAtEnd()) {
        ErrorFound = true;
        m_error_message = "Expected ) after arguments.";
      }
    }
  }
  return ParamVector;
}
std::vector<std::unique_ptr<AST::Expression>> RDParser::ConsumeArguments()
{
  std::vector<std::unique_ptr<AST::Expression>> Vector;
  while (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenClose>()) {
    auto Expression = std::move(ParseExpression());
    Vector.push_back(std::move(Expression));
    if (!Check(Ore::Parser::Token::TokenType::ParenClose)) {
      ConsumeToken(Ore::Parser::Token::TokenType::Comma, "expected comma between arguments");
      if (Check(Ore::Parser::Token::TokenType::ParenClose)) {
        ErrorFound = true;
        m_error_message = "Expected argument between commas.";
      }
    }
    if (isAtEnd()) {
      ErrorFound = true;
      m_error_message = "Expected ) after arguments.";
    }
  }
  return Vector;
}
std::unique_ptr<AST::Expression> RDParser::ParseExpression()
{
  auto Left = OrExpression();
  if (AdvanceIfMatchAny<Token::TokenType::Equals, Token::TokenType::PlusEquals, Token::TokenType::MinusEquals, Token::TokenType::AsteriskEquals, Token::TokenType::SlashEquals, Token::TokenType::ShiftLeftEquals, Token::TokenType::ShiftRightEquals, Token::TokenType::DoubleDotEquals>()) {
    AST::AssignmentExpression::Op Operator;
    switch (Previous.type()) {
    case Token::TokenType::Equals:
      Operator = AST::AssignmentExpression::Op::Assignment;
      break;
    case Token::TokenType::PlusEquals:
      Operator = AST::AssignmentExpression::Op::AddAssignment;
      break;
    case Token::TokenType::MinusEquals:
      Operator = AST::AssignmentExpression::Op::SubAssignment;
      break;
    case Token::TokenType::AsteriskEquals:
      Operator = AST::AssignmentExpression::Op::MultAssignment;
      break;
    case Token::TokenType::SlashEquals:
      Operator = AST::AssignmentExpression::Op::DivAssignment;
      break;
    case Token::TokenType::ShiftLeftEquals:
      Operator = AST::AssignmentExpression::Op::ShiftLeftAssignment;
      break;
    case Token::TokenType::ShiftRightEquals:
      Operator = AST::AssignmentExpression::Op::ShiftRightAssignment;
      break;
    case Token::TokenType::DoubleDotEquals:
      Operator = AST::AssignmentExpression::Op::ConcatAssignment;
      break;
    default:
      m_error_message = "Invalid assignment.";
      ErrorFound = true;
    }
    auto Right = ParseExpression();
    Left = std::make_unique<AST::AssignmentExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::OrExpression()
{
  auto Left = XorExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Or>()) {
    auto Right = XorExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), AST::BinaryExpression::Op::Or, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::XorExpression()
{
  auto Left = AndExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Xor>()) {
    auto Right = AndExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), AST::BinaryExpression::Op::Xor, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::AndExpression()
{
  auto Left = EqExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::And>()) {
    auto Right = EqExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), AST::BinaryExpression::Op::And, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::EqExpression()
{
  auto Left = CmpExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::EqualsEquals, Ore::Parser::Token::TokenType::ExclamationMarkEquals>()) {
    AST::BinaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::EqualsEquals:
      Operator = AST::BinaryExpression::Op::Equals;
      break;
    case Ore::Parser::Token::TokenType::ExclamationMarkEquals:
      Operator = AST::BinaryExpression::Op::NotEquals;
      break;
    }
    auto Right = CmpExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::CmpExpression()
{
  auto Left = ShfExpression();
  while (AdvanceIfMatchAny<Token::TokenType::LargerThan, Token::TokenType::LessThan, Token::TokenType::LessThanEquals, Token::TokenType::LargerThanEquals>()) {
    AST::BinaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::LargerThan:
      Operator = AST::BinaryExpression::Op::GreaterThan;
      break;
    case Ore::Parser::Token::TokenType::LessThan:
      Operator = AST::BinaryExpression::Op::LessThan;
      break;
    case Ore::Parser::Token::TokenType::LargerThanEquals:
      Operator = AST::BinaryExpression::Op::GreaterThanOrEquals;
      break;
    case Ore::Parser::Token::TokenType::LessThanEquals:
      Operator = AST::BinaryExpression::Op::LessThanOrEquals;
      break;
    }
    auto Right = ShfExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::ShfExpression()
{
  auto Left = AddExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ShiftLeft, Ore::Parser::Token::TokenType::ShiftRight>()) {
    AST::BinaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::ShiftLeft:
      Operator = AST::BinaryExpression::Op::ShiftLeft;
      break;
    case Ore::Parser::Token::TokenType::ShiftRight:
      Operator = AST::BinaryExpression::Op::ShiftRight;
      break;
    }
    auto Right = AddExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::AddExpression()
{
  auto Left = MulExpression();
  while (AdvanceIfMatchAny<Token::TokenType::Plus, Token::TokenType::Minus, Token::TokenType::DoubleDot>()) {
    AST::BinaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::Plus:
      Operator = AST::BinaryExpression::Op::Add;
      break;
    case Ore::Parser::Token::TokenType::Minus:
      Operator = AST::BinaryExpression::Op::Sub;
      break;
    case Ore::Parser::Token::TokenType::DoubleDot:
      Operator = AST::BinaryExpression::Op::StringConcat;
      break;
    }
    auto Right = MulExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::MulExpression()
{
  auto Left = ExpoExpression();
  while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Asterisk, Ore::Parser::Token::TokenType::Slash, Ore::Parser::Token::TokenType::Percent>()) {
    AST::BinaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::Asterisk:
      Operator = AST::BinaryExpression::Op::Mult;
      break;
    case Ore::Parser::Token::TokenType::Slash:
      Operator = AST::BinaryExpression::Op::Div;
      break;
    case Ore::Parser::Token::TokenType::Percent:
      Operator = AST::BinaryExpression::Op::Modulo;
      break;
    }
    auto Right = ExpoExpression();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), Operator, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::ExpoExpression()
{
  auto Left = Unary();
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::DoubleAsterisk>()) {
    auto Right = Unary();
    Left = std::make_unique<AST::BinaryExpression>(SourceRange {}, std::move(Left), AST::BinaryExpression::Op::Pow, std::move(Right));
  }
  return Left;
}
std::unique_ptr<AST::Expression> RDParser::Unary()
{
  if (AdvanceIfMatchAny<Token::TokenType::Hash, Token::TokenType::Not, Token::TokenType::Minus>()) {
    AST::UnaryExpression::Op Operator;
    switch (Previous.type()) {
    case Ore::Parser::Token::TokenType::Hash:
      Operator = AST::UnaryExpression::Op::Length;
      break;
    case Ore::Parser::Token::TokenType::Not:
      Operator = AST::UnaryExpression::Op::Not;
      break;
    case Ore::Parser::Token::TokenType::Minus:
      Operator = AST::UnaryExpression::Op::Negate;
      break;
    }
    return std::make_unique<AST::UnaryExpression>(SourceRange {}, Operator, Unary());
  }
  return Primitive();
}
std::unique_ptr<AST::Expression> RDParser::Primitive()
{
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Function>()) {
    return ParseFunction();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
    auto Expression = ParseExpression();
    ConsumeToken(Ore::Parser::Token::TokenType::ParenClose, "Expected ) after grouping expression");
    return Expression;
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::NilLiteral>()) {
    return std::make_unique<AST::NilLiteral>(SourceRange {});
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BoolLiteral>()) {
    return std::make_unique<AST::BooleanLiteral>(SourceRange {}, (Previous.value() == "true") ? true : false);
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::StringLiteral>()) {
    return std::make_unique<AST::StringLiteral>(SourceRange {}, Previous.value().substr(1, Previous.value().length() - 2));
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BracketOpen>()) {
    auto ExprVector = ConsumeElements();
    return std::make_unique<AST::ArrayExpression>(SourceRange {}, std::move(ExprVector));
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BracketClose>()) {
    ErrorFound = true;
    m_error_message = "Expected [ before ] taken.";
    return std::make_unique<AST::NilLiteral>(SourceRange {});
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::NumberLiteral>()) {
    return std::make_unique<AST::NumberLiteral>(SourceRange {}, convertToDouble(Previous.value()));
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyOpen>()) {
    return Object();
  } else if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyClose>()) {
    ErrorFound = true;
    m_error_message = "Expected { before }.";
    return std::make_unique<AST::NilLiteral>(SourceRange {});
  } else {
    return Call();
  }
}
std::vector<std::unique_ptr<AST::Expression>> RDParser::ConsumeElements()
{
  std::vector<std::unique_ptr<AST::Expression>> Vector;
  while (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BracketClose>()) {
    auto Expr = ParseExpression();
    if (!Check(Ore::Parser::Token::TokenType::BracketClose)) {
      ConsumeToken(Ore::Parser::Token::TokenType::Comma, "comma expected between array elements");
      if (Check(Ore::Parser::Token::TokenType::BracketClose)) {
        ErrorFound = true;
        m_error_message = "Expected elements between commas.";
      }
    }
    if (isAtEnd()) {
      ErrorFound = true;
      m_error_message = "Expected ] in array constructor.";
    }
    Vector.push_back(std::move(Expr));
  }
  return Vector;
}
double RDParser::convertToDouble(std::string const& str)
{
  if (str.size() < 2)
    return std::stod(str); // If the string is less than 2 characters, assume it's a double in base ten

  if (str.substr(0, 2) == "0b") {
    // Binary format
    long long int decimal = std::stoll(str.substr(2), nullptr, 2);
    return static_cast<double>(decimal);
  } else if (str.substr(0, 2) == "0x") {
    // Hexadecimal format
    long long int decimal = std::stoll(str.substr(2), nullptr, 16);
    return static_cast<double>(decimal);
  } else if (str.substr(0, 2) == "0o") {
    // Octal format
    long long int decimal = std::stoll(str.substr(2), nullptr, 8);
    return static_cast<double>(decimal);
  } else {
    // Base ten format
    return std::stod(str);
  }
}

std::unique_ptr<AST::Expression> RDParser::Call()
{
  if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Identifier>()) {
    auto Aux = std::make_unique<AST::Identifier>(SourceRange {}, Previous.value());
    if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
      auto args = ConsumeArguments();
      auto call = std::make_unique<AST::CallExpression>(SourceRange {}, std::move(Aux), std::move(args));
      if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
        ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
        auto member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(call), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
        while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
          ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
          member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
        }
        if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
          auto args = ConsumeArguments();
          return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args));
        }
        if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Colon>()) {
          ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected identifier");
          member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
          ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected (");
          auto args = ConsumeArguments();
          return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args), true);
        }
        return member;
      }
      return call;
    }
    if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BracketOpen>()) {
      auto Index = ParseExpression();
      ConsumeToken(Ore::Parser::Token::TokenType::BracketClose, "] expected");
      auto Called = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(Aux), std::move(Index), true);
      while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::BracketOpen>()) {
        Index = ParseExpression();
        Called = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(Called), std::move(Index), true);
        ConsumeToken(Ore::Parser::Token::TokenType::BracketClose, "] expected");
      }
      return Called;
    }
    if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
      ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
      auto member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(Aux), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
      while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
        ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
        member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
      }
      if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
        auto args = ConsumeArguments();
        auto call = std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args));
        if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
          ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
          auto member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(call), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
          while (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Dot>()) {
            ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Error: Expected identifier");
            member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
          }
          if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::ParenOpen>()) {
            auto args = ConsumeArguments();
            return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args));
          }
          if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Colon>()) {
            ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected identifier");
            member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
            ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected (");
            auto args = ConsumeArguments();
            return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args), true);
          }
          return member;
        }
        return call;
      }
      if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Colon>()) {
        ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected identifier");
        member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(member), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
        ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected (");
        auto args = ConsumeArguments();
        return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args), true);
      }
      return member;
    }
    if (AdvanceIfMatchAny<Ore::Parser::Token::TokenType::Colon>()) {
      ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "expected identifier");
      auto member = std::make_unique<AST::MemberExpression>(SourceRange {}, std::move(Aux), std::make_unique<AST::Identifier>(SourceRange {}, Previous.value()));
      ConsumeToken(Ore::Parser::Token::TokenType::ParenOpen, "expected (");
      auto args = ConsumeArguments();
      return std::make_unique<AST::CallExpression>(SourceRange {}, std::move(member), std::move(args), true);
    }
    return Aux;
  } else {
    Advance();
    m_error_message = fmt::format("Unexpected token '{}'.", Previous.value());
    ErrorFound = true;
    return std::make_unique<AST::BooleanLiteral>(SourceRange {}, false);
  }
}
std::unique_ptr<AST::Expression> RDParser::Object()
{
  std::map<std::string, std::unique_ptr<AST::Expression>> Members;
  while (!AdvanceIfMatchAny<Ore::Parser::Token::TokenType::CurlyClose>()) {
    ConsumeToken(Ore::Parser::Token::TokenType::Identifier, "Expected identifier");
    if (Previous.type() == Ore::Parser::Token::TokenType::Identifier) {
      auto key = Previous.value();
      ConsumeToken(Ore::Parser::Token::TokenType::Colon, "Expected : ");
      Members[key] = ParseExpression();
      if (!Check(Ore::Parser::Token::TokenType::CurlyClose)) {
        ConsumeToken(Ore::Parser::Token::TokenType::Comma, "Expected comma between members");
      }
    } else
      break;
  }
  return std::make_unique<AST::ObjectExpression>(SourceRange {}, std::move(Members));
}
}
/*
Arrays
Maps
Member expression
*/
