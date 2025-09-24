#include <RTypeNet/Subplatform/Poll.hpp>

int rtype::network::subplatform::poll(PollFD fds[], NFDS nfds, int timeout) noexcept
{
    return ::poll(reinterpret_cast<struct pollfd *>(fds), nfds, timeout);
}
