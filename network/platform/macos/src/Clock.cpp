#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../../../api/include/Clock.hpp"
#else
    #include "Clock.hpp"
#endif

#include "SubplatformClock.hpp"

RTYPE_NET_API std::uint64_t rtype::network::now_ns() noexcept
{
    return subplatform::now_ns();
}
