#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>

namespace rtype::network {

[[nodiscard]] RTYPE_NET_API Socket listen(const Endpoint &e, Protocol p);

}
