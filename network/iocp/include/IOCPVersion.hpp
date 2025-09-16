#pragma once

#include "NetworkApi.hpp"

#define R_TYPE_NETWORK_IOCP_VERSION "0.0.1"

namespace rtype::network {

R_TYPE_NETWORK_API const char *rtype_network_platform_name();
R_TYPE_NETWORK_API const char *rtype_network_platform_version();

}// namespace rtype::network
