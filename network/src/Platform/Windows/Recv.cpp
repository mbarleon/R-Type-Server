#include <RTypeNet/Recv.hpp>
#include <winsock2.h>

RTYPE_NET_API ssize_t rtype::network::recv(Handle handle, void *buffer, BufLen length, int flags) noexcept
{
    int ret = ::recv(handle, static_cast<char *>(buffer), static_cast<int>(length), flags);
    return static_cast<ssize_t>(ret);
}
