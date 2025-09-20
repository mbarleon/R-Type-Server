#include <RTypeNet/Core/Version.hpp>
#include <RTypeNet/Subplatform/POSIX/Version.hpp>
#include <RTypeNet/Subplatform/Version.hpp>
#include <string>

RTYPE_NET_API const char *rtype::network::subplatform::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_POSIX;
}

RTYPE_NET_API const char *rtype::network::subplatform::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_POSIX;
}

RTYPE_NET_API std::string rtype::network::subplatform::full_version() noexcept
{
    return std::string(RTYPE_NET_PLATFORM_NAME_POSIX " subplatform Ver. " RTYPE_NET_PLATFORM_VER_POSIX "\n") + core::full_version();
}
