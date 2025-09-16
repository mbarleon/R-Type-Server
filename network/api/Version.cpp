#include <sstream>
#include "RTypeNetworkVersion.hpp"
#include "RTypePlatfomSpecificDefines.hpp"

R_TYPE_NETWORK_API char *rtype_full_network_version(void)
{
    std::ostringstream oss;

    oss << "R-Type network ver. " << R_TYPE_NETWORK_API_VERSION <<
        "\n" << rtype_network_platform_name() << " implementation ver. " << rtype_network_platform_version() <<
        "\nCore ver. " << R_TYPE_NERWORK_CORE_VERSION;
    return RTYPE_AGNOSTIC_STRDUP_IMPL(oss.str().c_str());
}

R_TYPE_NETWORK_API const char *rtype_network_version(void)
{
    return R_TYPE_NETWORK_API_VERSION;
}
