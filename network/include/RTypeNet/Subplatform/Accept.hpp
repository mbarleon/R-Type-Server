#pragma once

#include <RTypeNet/Api.hpp>
#include <RTypeNet/Interfaces.hpp>

namespace rtype::network::subplatform {

[[nodiscard]] RTYPE_NET_API Socket accept(Handle serverHandle);

}
