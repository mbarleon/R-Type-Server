#include <RTypeNet/Subplatform/Send.hpp>
#include <sys/socket.h>
#include <sys/types.h>

RTYPE_NET_API ssize_t rtype::network::subplatform::send(Handle handle, const void *buffer, BufLen length, int flags) noexcept
{
    return ::send(handle, buffer, length, flags);
}
