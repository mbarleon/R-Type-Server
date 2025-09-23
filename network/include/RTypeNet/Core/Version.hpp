#pragma once

#include <RTypeNet/Api.hpp>
#include <string>

#define RTYPE_NET_CORE_VER "0.0.1"

namespace rtype::network::core {

[[nodiscard]] RTYPE_NET_API const char *version() noexcept;
[[nodiscard]] RTYPE_NET_API std::string full_version() noexcept;

}// namespace rtype::network::core
