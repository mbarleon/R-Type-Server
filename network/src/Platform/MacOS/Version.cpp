#include <RTypeNet/Platform/MacOS/Version.hpp>
#include <RTypeNet/Subplatform/Version.hpp>
#include <RTypeNet/Version.hpp>
#include <string>

RTYPE_NET_API const char *rtype::network::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_MAC;
}

RTYPE_NET_API const char *rtype::network::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_MAC;
}

RTYPE_NET_API std::string rtype::network::full_version() noexcept
{
    return std::string("R-Type network for " RTYPE_NET_PLATFORM_NAME_MAC " platform Ver. " RTYPE_NET_PLATFORM_VER_MAC "\n")
        + subplatform::full_version();
}
