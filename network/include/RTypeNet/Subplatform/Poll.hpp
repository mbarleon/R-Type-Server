#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>
#include <limits>

namespace rtype::network::subplatform {

constexpr auto NFDS_MAX = (std::numeric_limits<NFDS>::max)();
constexpr auto NFDS_MIN = (std::numeric_limits<NFDS>::min)();

[[nodiscard]] RTYPE_NET_API int poll(PollFD fds[], NFDS nfds, int timeout) noexcept;

}// namespace rtype::network
