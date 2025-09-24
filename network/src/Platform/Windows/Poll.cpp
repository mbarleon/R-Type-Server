#include <RTypeNet/Poll.hpp>

int rtype::network::poll(PollFD fds[], NFDS nfds, int timeout) noexcept
{
    return ::WSAPoll(reinterpret_cast<WSAPOLLFD *>(fds), static_cast<ULONG>(nfds), timeout);
}
