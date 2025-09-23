#pragma once

#include <RTypeNet/Api.hpp>
#include <string>

namespace rtype::network {

[[nodiscard]] RTYPE_NET_API const char *name() noexcept;
[[nodiscard]] RTYPE_NET_API const char *version() noexcept;
[[nodiscard]] RTYPE_NET_API std::string full_version() noexcept;

}// namespace rtype::network
