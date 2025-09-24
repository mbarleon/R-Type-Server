#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>
#include <limits>

namespace rtype::network {

constexpr auto BUF_LEN_MAX = (std::numeric_limits<BufLen>::max)();
constexpr auto BUF_LEN_MIN = (std::numeric_limits<BufLen>::min)();

}
