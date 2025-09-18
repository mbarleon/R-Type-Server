#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/MacOSVersion.hpp"
#else
    #include <MacOSVersion.hpp>
#endif

#include <PlatformVersion.hpp>
#include <SubplatformVersion.hpp>
#include <string>

RTYPE_NET_API const char *rtype::network::platform::name() noexcept
{
    return RTYPE_NET_PLATFORM_NAME_MAC;
}

RTYPE_NET_API const char *rtype::network::platform::version() noexcept
{
    return RTYPE_NET_PLATFORM_VER_MAC;
}

RTYPE_NET_API std::string rtype::network::platform::full_version() noexcept
{
    return std::string(RTYPE_NET_PLATFORM_NAME_MAC " platform Ver. " RTYPE_NET_PLATFORM_VER_MAC "\n") + subplatform::full_version();
}
