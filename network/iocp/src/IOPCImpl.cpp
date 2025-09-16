#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
    #include "../include/IOCPClock.hpp"
#else
    #include "IOCPClock.hpp"
#endif

#include <memory>

namespace rtype::network {

R_TYPE_NETWORK_API std::unique_ptr<IClock> make_clock()
{
    return std::make_unique<IOCPClock>();
}

}// namespace rtype::network
