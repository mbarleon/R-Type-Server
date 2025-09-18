#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/POSIXVersion.hpp"
#else
    #include "POSIXVersion.hpp"
#endif

#include "CoreVersion.hpp"
#include "SubplatformVersion.hpp"
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
