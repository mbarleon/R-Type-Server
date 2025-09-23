#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>

namespace rtype::network {

RTYPE_NET_API void disconnect(Socket &sock) noexcept;

}// namespace rtype::network
