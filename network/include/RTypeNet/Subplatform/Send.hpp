#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/BufLenLimits.hpp>
#include <RTypeNet/Interfaces.hpp>
#include <RTypeNet/ssize_t.hpp>

namespace rtype::network::subplatform {

[[nodiscard]] RTYPE_NET_API ssize_t send(Handle handle, const void *buffer, BufLen length, int flags) noexcept;

[[nodiscard]] RTYPE_NET_API ssize_t sendto(Handle handle, const void *buffer, BufLen length, int flags, const Endpoint &to) noexcept;

}// namespace rtype::network::subplatform
