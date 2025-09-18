#pragma once

#include <NetworkApi.hpp>
#include <string>

namespace rtype::network::subplatform {

RTYPE_NET_API const char *name() noexcept;
RTYPE_NET_API const char *version() noexcept;
RTYPE_NET_API std::string full_version() noexcept;

}// namespace rtype::network::subplatform
