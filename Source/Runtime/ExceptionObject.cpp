#include "ExceptionObject.h"
#include "../Interpreter.h"
#include <sstream>

namespace Ore {
ExceptionObject::ExceptionObject(std::string const& type, std::string const& message)
    : m_type(std::move(type))
    , m_message(std::move(message))
{

  populate_backtrace();

  put(PropertyKey("type"), ore_string(heap(), type));
  put(PropertyKey("message"), ore_string(heap(), message));
}

void ExceptionObject::populate_backtrace()
{
  for (
      auto scope = interpreter().function_scopes().rbegin();
      scope != interpreter().function_scopes().rend();
      ++scope) {
    m_backtrace.push_back({ scope->name });
  }

  m_backtrace.push_back({ "global" });
}

}
