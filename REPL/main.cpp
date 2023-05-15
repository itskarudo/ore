#include "ScriptGlobalObjectShape.h"
#include <Ore.h>
#include <cxxopts.hpp>
#include <fstream>

static void log_exception(Ore::ExceptionObject& exception)
{
  std::cout << "\033[1m\033[31m" << exception.type() << "\033[0m: " << exception.message() << std::endl;
}

static void parse_and_run(Ore::Interpreter& interpreter, std::string_view source, bool dump_ast)
{
  Ore::Parser::Lexer lexer(source);
  Ore::Parser::RDParser parser(lexer);

  auto program = parser.parse();

  if (dump_ast)
    program->dump();

  auto return_value = interpreter.run(*program);

  if (return_value.is_exception()) {
    auto* exception = reinterpret_cast<Ore::ExceptionObject*>(return_value.value().as_object());
    log_exception(*exception);
  }
}

int main(int argc, char** argv)
{
  cxxopts::Options options("orepl", "The ore language repl.");

  options.positional_help("[file] [ARGS...]");

  // clang-format off
  options.add_options()
    ("d,dump", "Dump the script AST")
    ("script", "Ore script to execute", cxxopts::value<std::string>())
    ("passed_args", "Script arguments", cxxopts::value<std::vector<std::string>>())
    ("h,help", "Print help");

  // clang-format on
  options.parse_positional({ "script", "passed_args" });

  auto result = options.parse(argc, argv);
  bool repl_mode = !result.count("script");
  bool dump_ast = result.count("dump");

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  auto interpreter = Ore::Interpreter::create<ScriptGlobalObjectShape>();

  if (repl_mode) {
    // TODO: REPL loop
    assert(false);
  } else {
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

    parse_and_run(*interpreter, std::move(string_source), dump_ast);
  }
}
