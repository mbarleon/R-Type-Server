#include <RTypeNet/Recv.hpp>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

RTYPE_NET_API ssize_t rtype::network::recv(Handle handle, void *buffer, BufLen length, int flags) noexcept
{
    int ret = ::recv(handle, static_cast<char *>(buffer), static_cast<int>(length), flags);
    return static_cast<ssize_t>(ret);
}

RTYPE_NET_API ssize_t rtype::network::recvfrom(Handle handle, void *buffer, BufLen length, int flags, Endpoint &from) noexcept
{
    sockaddr_storage addr{};
    int addrlen = sizeof(addr);
    int ret =
        ::recvfrom(handle, static_cast<char *>(buffer), static_cast<int>(length), flags, reinterpret_cast<sockaddr *>(&addr), &addrlen);
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
    return static_cast<ssize_t>(ret);
}
