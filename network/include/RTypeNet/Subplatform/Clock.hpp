#pragma once

#include <RTypeNet/Api.hpp>
#include <cstdint>

namespace rtype::network::subplatform {

[[nodiscard]] RTYPE_NET_API std::uint64_t now_ns() noexcept;

}// namespace rtype::network::subplatform
