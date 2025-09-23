#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/BufLenLimits.hpp>
#include <RTypeNet/Interfaces.hpp>

namespace rtype::network {

[[nodiscard]] RTYPE_NET_API ssize_t recv(Handle handle, void *buffer, BufLen length, int flags) noexcept;

}
