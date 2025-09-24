#include <RTypeNet/Subplatform/Poll.hpp>

RTYPE_NET_API int rtype::network::subplatform::poll(PollFD fds[], NFDS nfds, int timeout) noexcept
{
    return ::poll(reinterpret_cast<struct pollfd *>(fds), nfds, timeout);
}
