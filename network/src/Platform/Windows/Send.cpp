#include <RTypeNet/Send.hpp>
#include <winsock2.h>

RTYPE_NET_API ssize_t rtype::network::send(Handle handle, const void *buffer, BufLen length, int flags) noexcept
{
    int ret = ::send(handle, static_cast<const char *>(buffer), static_cast<int>(length), flags);
    return static_cast<ssize_t>(ret);
}
