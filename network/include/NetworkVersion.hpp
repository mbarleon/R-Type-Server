#pragma once

#include "NetworkApi.hpp"
#include <string>

#define R_TYPE_NETWORK_API_VERSION "0.0.1"

namespace rtype::network {

R_TYPE_NETWORK_API std::string rtype_full_network_version();
R_TYPE_NETWORK_API const char *rtype_network_version();

}// namespace rtype::network
