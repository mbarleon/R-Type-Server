#include <RTypeNet/Platform/Linux/Version.hpp>
#include <RTypeNet/Subplatform/Version.hpp>
#include <RTypeNet/Version.hpp>
#include <string>

RTYPE_NET_API const char *rtype::network::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_LIN;
}

RTYPE_NET_API const char *rtype::network::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_LIN;
}

RTYPE_NET_API std::string rtype::network::full_version() noexcept
{
    return std::string("R-Type network for " RTYPE_NET_PLATFORM_NAME_LIN " platform Ver. " RTYPE_NET_PLATFORM_VER_LIN "\n")
        + subplatform::full_version();
}
