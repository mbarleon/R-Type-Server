#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/IOCPVersion.hpp"
#else
    #include "IOCPVersion.hpp"
#endif

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_name()
{
    return "IOCP";
}

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_version()
{
    return R_TYPE_NETWORK_IOCP_VERSION;
}
