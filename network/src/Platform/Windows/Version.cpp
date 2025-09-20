#include <RTypeNet/Core/Version.hpp>
#include <RTypeNet/Platform/Windows/Version.hpp>
#include <RTypeNet/Version.hpp>
#include <string>

RTYPE_NET_API const char *rtype::network::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_WIN;
}

RTYPE_NET_API const char *rtype::network::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_WIN;
}

RTYPE_NET_API std::string rtype::network::full_version() noexcept
{
    return std::string("R-Type network for " RTYPE_NET_PLATFORM_NAME_WIN " platform Ver. " RTYPE_NET_PLATFORM_VER_WIN "\n")
        + core::full_version();
}
