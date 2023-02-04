#include "ExceptionObject.h"

namespace Ore {
ExceptionObject::ExceptionObject(std::string const& type, std::string const& message)
    : m_type(std::move(type))
    , m_message(std::move(message))
{
  put(PropertyKey("type"), ore_string(heap(), type));
  put(PropertyKey("message"), ore_string(heap(), message));
}
}
