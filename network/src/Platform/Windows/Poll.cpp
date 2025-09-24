#include <RTypeNet/Poll.hpp>

RTYPE_NET_API int rtype::network::poll(PollFD fds[], NFDS nfds, int timeout) noexcept
{
    return ::WSAPoll(reinterpret_cast<WSAPOLLFD *>(fds), static_cast<ULONG>(nfds), timeout);
}
