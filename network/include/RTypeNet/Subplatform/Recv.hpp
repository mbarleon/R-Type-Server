#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/BufLenLimits.hpp>
#include <RTypeNet/Interfaces.hpp>
#include <RTypeNet/ssize_t.hpp>

namespace rtype::network::subplatform {

[[nodiscard]] RTYPE_NET_API ssize_t recv(Handle handle, void *buffer, BufLen length, int flags) noexcept;

[[nodiscard]] RTYPE_NET_API ssize_t recvfrom(Handle handle, void *buffer, BufLen length, int flags, Endpoint &from) noexcept;

}// namespace rtype::network::subplatform
