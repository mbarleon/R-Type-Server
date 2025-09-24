#pragma once

#include <RTypeSrv/Api.hpp>
#include <string>

#define RTYPE_SRV_VER "0.0.1"

namespace rtype::srv {

[[nodiscard]] RTYPE_SRV_API const char *version() noexcept;
[[nodiscard]] RTYPE_SRV_API std::string full_version() noexcept;

}// namespace rtype::network
