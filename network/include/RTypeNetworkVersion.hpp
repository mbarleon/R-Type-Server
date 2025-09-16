#pragma once

#include "RTypeNetworkApi.hpp"

#define R_TYPE_NETWORK_API_VERSION "0.0.1"

#if defined(__cplusplus)
extern "C" {
#endif

R_TYPE_NETWORK_API const char *rtype_full_network_version();
R_TYPE_NETWORK_API const char *rtype_network_version();

#if defined(__cplusplus)
}
#endif
