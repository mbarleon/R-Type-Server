#include "POSIXClock.hpp"
#include <memory>

namespace rtype::network {

R_TYPE_NETWORK_API std::unique_ptr<IClock> make_clock()
{
    return std::make_unique<POSIXClock>();
}

}// namespace rtype::network
