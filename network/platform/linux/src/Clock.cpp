#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../../../api/include/Clock.hpp"
#else
    #include <Clock.hpp>
#endif

#include <SubplatformClock.hpp>
#include <SubplatformForwardFunction.hpp>

RTYPE_NET_FWD_FUNC(now_ns, noexcept)
