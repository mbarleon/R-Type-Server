#include <RTypeNet/Subplatform/Recv.hpp>
#include <sys/socket.h>
#include <sys/types.h>

RTYPE_NET_API ssize_t rtype::network::subplatform::recv(Handle handle, void *buffer, BufLen length, int flags) noexcept
{
    return ::recv(handle, buffer, length, flags);
}
