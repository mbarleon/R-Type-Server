#include <RTypeNet/Subplatform/Recv.hpp>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

RTYPE_NET_API ssize_t rtype::network::subplatform::recv(Handle handle, void *buffer, BufLen length, int flags) noexcept
{
    return ::recv(handle, buffer, length, flags);
}

RTYPE_NET_API ssize_t rtype::network::subplatform::recvfrom(Handle handle, void *buffer, BufLen length, int flags, Endpoint &from) noexcept
{
    sockaddr_storage addr{};
    socklen_t addrlen = sizeof(addr);
    ssize_t ret = ::recvfrom(handle, buffer, length, flags, reinterpret_cast<sockaddr *>(&addr), &addrlen);
    if (ret >= 0) {
        if (addr.ss_family == AF_INET) {
            auto *a4 = reinterpret_cast<sockaddr_in *>(&addr);
            from.port = ntohs(a4->sin_port);
            from.ip.fill(0);
            std::memcpy(from.ip.data(), &a4->sin_addr, 4);
        } else if (addr.ss_family == AF_INET6) {
            auto *a6 = reinterpret_cast<sockaddr_in6 *>(&addr);
            from.port = ntohs(a6->sin6_port);
            std::memcpy(from.ip.data(), &a6->sin6_addr, 16);
        }
    }
    return ret;
}
