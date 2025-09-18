#pragma once

#include "NetworkApi.hpp"
#include <cstdint>

namespace rtype::network {

RTYPE_NET_API std::uint64_t now_ns() noexcept;

}// namespace rtype::network
