#pragma once

#include <NetworkApi.hpp>
#include <string>

#define RTYPE_NET_NAME "R-Type Network"
#define RTYPE_NET_VER "0.0.1"

namespace rtype::network {

RTYPE_NET_API const char *version() noexcept;
RTYPE_NET_API std::string full_version() noexcept;

}// namespace rtype::network
