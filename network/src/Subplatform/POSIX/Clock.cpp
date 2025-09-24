#include <RTypeNet/Subplatform/Clock.hpp>
#include <chrono>

RTYPE_NET_API std::uint64_t rtype::network::subplatform::now_ns() noexcept
{
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}
