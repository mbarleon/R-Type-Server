#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>
#include <limits>

namespace rtype::network {

RTYPE_NET_API constexpr auto BUF_LEN_MAX = std::numeric_limits<BufLen>::max();
RTYPE_NET_API constexpr auto BUF_LEN_MIN = std::numeric_limits<BufLen>::min();

}
