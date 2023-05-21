#include "REPLGlobalObjectShape.h"
#include <Ore.h>
#include <algorithm>
#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fstream>
#include <readline/history.h>
#include <readline/readline.h>

static bool s_fail_repl = false;
static bool s_dump_ast = false;
static int s_line_number = 1;
static int s_repl_line_level = 0;
static std::string s_history_path = {};

static std::string get_prompt()
{
  if (s_repl_line_level == 0) {
    return fmt::format("\033[32m[{}]:\033[0m ", s_line_number);
  } else {
    return "\033[32m...:\033[0m ";
  }
}

static void log_exception(Ore::ExceptionObject& exception)
{
  std::cout << "\033[1m\033[31mBacktrace\033[0m (most recent calls first):" << std::endl;

  std::cout << "\033[34m";
  for (auto frame : exception.backtrace()) {
    std::cout << "  " << frame.function_name << std::endl;
  }
  std::cout << "\033[1m\033[31m";

  for (int i = 0; i < 30; ++i)
    std::cout << '-';

  std::cout << "\033[0m\n";

  std::cout << "\033[1m\033[31m" << exception.type() << "\033[0m: " << exception.message() << std::endl;
}

static bool is_whitespace(std::string const& str)
{
  return std::all_of(str.begin(), str.end(), [](char c) {
    return std::isspace(c);
  });
}

static void parse_and_run(Ore::Interpreter& interpreter, std::string source)
{
  Ore::Parser::Lexer lexer(source);
  Ore::Parser::RDParser parser(lexer);

  auto program = parser.parse();

  if (s_dump_ast)
    program->dump();

  auto return_value = interpreter.run(*program);

  if (return_value.is_exception()) {
    auto* exception = reinterpret_cast<Ore::ExceptionObject*>(return_value.value().as_object());
    log_exception(*exception);
  }
}

std::optional<std::string> read_next_piece()
{
  std::stringstream piece;

  size_t open_parens = 0;
  size_t open_brackets = 0;
  size_t open_curlies = 0;

  do {

    assert(open_parens >= 0);
    assert(open_brackets >= 0);
    assert(open_curlies >= 0);

    auto* c_line = readline(get_prompt().c_str());
    if (!c_line) {
      s_fail_repl = true;
      return {};
    }

    add_history(c_line);

    std::string line(c_line);
    piece << line;

    Ore::Parser::Lexer lexer(line);

    using Ore::Parser::Token;

    for (auto token = lexer.next();
         token.type() != Token::TokenType::Eof;
         token = lexer.next()) {

      switch (token.type()) {
      case Token::TokenType::ParenOpen:
        open_parens++;
        s_repl_line_level++;
        break;
      case Token::TokenType::ParenClose:
        open_parens--;
        s_repl_line_level--;
        break;
      case Token::TokenType::BracketOpen:
        open_brackets++;
        s_repl_line_level++;
        break;
      case Token::TokenType::BracketClose:
        open_brackets--;
        s_repl_line_level--;
        break;
      case Token::TokenType::CurlyOpen:
        open_curlies++;
        s_repl_line_level++;
        break;
      case Token::TokenType::CurlyClose:
        open_curlies--;
        s_repl_line_level--;
        break;
      default:
        break;
      }
    }

  } while (open_parens + open_brackets + open_curlies > 0);

  return piece.str();
}

int main(int argc, char** argv)
{
  cxxopts::Options options("orepl", "The ore language repl.");

  options.positional_help("[file] [ARGS...]");

  // clang-format off
  options.add_options()
    ("d,dump", "Dump the script AST")
    ("c,evaluate", "Evaluate argument as script", cxxopts::value<std::string>())
    ("h,help", "Print help")
    ("script", "Ore script to execute", cxxopts::value<std::string>())
    ("passed_args", "Script arguments", cxxopts::value<std::vector<std::string>>());

  // clang-format on
  options.parse_positional({ "script", "passed_args" });

  auto result = options.parse(argc, argv);
  bool repl_mode = !result.count("script") && !result.count("evaluate");

  s_history_path = fmt::format("{}/.ore_history", std::getenv("HOME"));
  s_dump_ast = result.count("dump");

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  auto interpreter = Ore::Interpreter::create<REPLGlobalObjectShape>();

  if (repl_mode) {

    using_history();
    read_history(s_history_path.c_str());

    rl_startup_hook = []() -> int {
      std::stringstream indents;

      for (int i = 0; i < s_repl_line_level; i++)
        indents << "  ";

      rl_insert_text(indents.str().c_str());

      return 0;
    };

    while (!s_fail_repl) {
      auto piece = read_next_piece();

      if (!piece.has_value() || is_whitespace(piece.value()))
        continue;

      s_line_number++;

      parse_and_run(*interpreter, piece.value());

      std::cout << "\033[1m" << interpreter->block_result() << std::endl;
      interpreter->set_block_result(Ore::ore_nil());
    }

    write_history(s_history_path.c_str());

  } else if (result.count("script")) {
    auto& script = result["script"].as<std::string>();

    std::vector<std::string> args = { script };

    if (result.count("passed_args")) {
      auto& passed_args = result["passed_args"].as<std::vector<std::string>>();
      args.insert(args.end(), passed_args.begin(), passed_args.end());
    }

    auto* args_array = interpreter->heap().allocate<Ore::ArrayObject>();

    for (auto& arg : args)
      args_array->elements().push_back(interpreter->heap().allocate<Ore::PrimitiveString>(arg));

    interpreter->global_object()->put(Ore::PropertyKey("args"), args_array);

    std::ifstream file(script);
    if (!file) {
      std::cout << "file '" << script << "' does not exist" << std::endl;
      return -1;
    }

    std::string string_source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    parse_and_run(*interpreter, std::move(string_source));
  } else {
    auto eval = result["evaluate"].as<std::string>();
    parse_and_run(*interpreter, std::move(eval));
  }
}
