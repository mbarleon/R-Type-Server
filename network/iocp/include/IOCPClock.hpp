#pragma once

#include "Clock.hpp"
#include <chrono>

#if defined(_WIN32)
    #include <windows.h>
#endif

namespace rtype::network {

class R_TYPE_NETWORK_HIDE_FROM_ABI IOCPClock final : public IClock
{
    public:
        constexpr explicit IOCPClock() = default;
        ~IOCPClock() noexcept override = default;

        std::uint64_t now_ns() override
        {
#if defined(_WIN32)
            LARGE_INTEGER freq, ctr;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&ctr);
            return static_cast<std::uint64_t>((ctr.QuadPart * 1000000000ULL) / freq.QuadPart);
#else
            return 0;
#endif
        }
};

}// namespace rtype::network
