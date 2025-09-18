#include "CoreVersion.hpp"

RTYPE_NET_API const char *rtype::network::core::version() noexcept
{
    return RTYPE_NET_CORE_VER;
}

RTYPE_NET_API std::string rtype::network::core::full_version() noexcept
{
    return std::string("Core Ver. " RTYPE_NET_CORE_VER);
}
