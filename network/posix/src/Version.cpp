#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/POSIXVersion.hpp"
#else
    #include "POSIXVersion.hpp"
#endif

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_name()
{
    return "POSIX";
}

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_version()
{
    return R_TYPE_NETWORK_POSIX_VERSION;
}
