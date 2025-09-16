#include "IOCPVersion.hpp"

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_name()
{
    return "IOCP";
}

R_TYPE_NETWORK_API const char *rtype::network::rtype_network_platform_version()
{
    return R_TYPE_NETWORK_IOCP_VERSION;
}
