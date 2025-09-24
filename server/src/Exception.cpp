#include <RTypeSrv/Exception.hpp>

namespace rtype::srv {

const char *Exception::what() const noexcept
{
    return _what.c_str();
}

const char *Exception::where() const noexcept
{
    return _where.c_str();
}

}// namespace rtype::srv
