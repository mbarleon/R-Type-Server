#pragma once

#include "CoreVersion.hpp"
#include "RTypeNetworkApi.hpp"

#if defined(_WIN32) && defined(_MSC_VER)
    #include "IOCPVersion.hpp"
#elif defined(__unix__) || defined(__APPLE__) || defined(__GNUC__)
    #include "POSIXVersion.hpp"
#else
    #error "Unsupported platform"
#endif

#define R_TYPE_NETWORK_API_VERSION "0.0.1"

#if defined(__cplusplus)
extern "C" {
#endif

R_TYPE_NETWORK_API char *rtype_full_network_version(void);
R_TYPE_NETWORK_API const char *rtype_network_version(void);

#if defined(__cplusplus)
}
#endif
