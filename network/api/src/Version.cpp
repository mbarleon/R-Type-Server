#include "NetworkVersion.hpp"
#include "PlatformVersion.hpp"
#include <string>

RTYPE_NET_API const char *rtype::network::version() noexcept
{
    return RTYPE_NET_VER;
}

RTYPE_NET_API std::string rtype::network::full_version() noexcept
{
    return std::string(RTYPE_NET_NAME " Ver. " RTYPE_NET_VER "\n") + platform::full_version();
}
