#pragma once

#include "Clock.hpp"
#include <chrono>

namespace rtype::network {

class R_TYPE_NETWORK_HIDE_FROM_ABI POSIXClock final : public IClock
{
    public:
        constexpr explicit POSIXClock() = default;
        ~POSIXClock() noexcept override = default;

        std::uint64_t now_ns() override
        {
            return static_cast<uint64_t>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        }
};

}// namespace rtype::network
