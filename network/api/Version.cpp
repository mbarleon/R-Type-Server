#include "RTypeNetworkVersion.hpp"
#include "RTypePlatfomSpecificDefines.hpp"
#include <sstream>

#include "CoreVersion.hpp"
#if defined(_WIN32) && defined(_MSC_VER)
    #include "IOCPVersion.hpp"
#elif defined(__unix__) || defined(__APPLE__) || defined(__GNUC__)
    #include "POSIXVersion.hpp"
#else
    #error "Unsupported platform"
#endif

R_TYPE_NETWORK_API char *rtype_full_network_version()
{
    std::ostringstream oss;

    oss << "R-Type network ver. " << R_TYPE_NETWORK_API_VERSION << "\n"
        << rtype_network_platform_name() << " implementation ver. " << rtype_network_platform_version() << "\nCore ver. " << R_TYPE_NETWORK_CORE_VERSION;
    return RTYPE_AGNOSTIC_STRDUP_IMPL(oss.str().c_str());
}

R_TYPE_NETWORK_API const char *rtype_network_version()
{
    return R_TYPE_NETWORK_API_VERSION;
}
