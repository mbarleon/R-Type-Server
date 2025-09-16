#pragma once

#include "NetworkApi.hpp"
#include <cstdint>

namespace rtype::network {

class R_TYPE_NETWORK_API IClock
{
    public:
        virtual ~IClock() = default;
        virtual std::uint64_t now_ns() = 0;
};

}// namespace rtype::network
