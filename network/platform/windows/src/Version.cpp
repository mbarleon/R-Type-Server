#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/WindowsVersion.hpp"
#else
    #include "WindowsVersion.hpp"
#endif

#include "CoreVersion.hpp"
#include "PlatformVersion.hpp"
#include <string>

RTYPE_NET_API const char *rtype::network::platform::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_WIN;
}

RTYPE_NET_API const char *rtype::network::platform::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_WIN;
}

RTYPE_NET_API std::string rtype::network::platform::full_version() noexcept
{
    return std::string(RTYPE_NET_PLATFORM_NAME_WIN " platform Ver. " RTYPE_NET_PLATFORM_VER_WIN "\n") + core::full_version();
}
