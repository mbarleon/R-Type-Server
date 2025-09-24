#include <RTypeNet/Send.hpp>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

RTYPE_NET_API ssize_t rtype::network::send(Handle handle, const void *buffer, BufLen length, int flags) noexcept
{
    int ret = ::send(handle, static_cast<const char *>(buffer), static_cast<int>(length), flags);
    return static_cast<ssize_t>(ret);
}

RTYPE_NET_API ssize_t rtype::network::sendto(Handle handle, const void *buffer, BufLen length, int flags, const Endpoint &to) noexcept
{
    sockaddr_storage addr{};
    int addrlen = 0;
    if (to.ip[4] || to.ip[5] || to.ip[6] || to.ip[7] || to.ip[8]) {
        sockaddr_in6 *a6 = reinterpret_cast<sockaddr_in6 *>(&addr);
        a6->sin6_family = AF_INET6;
        a6->sin6_port = htons(to.port);
        std::memcpy(&a6->sin6_addr, to.ip.data(), 16);
        addrlen = sizeof(sockaddr_in6);
    } else {
        sockaddr_in *a4 = reinterpret_cast<sockaddr_in *>(&addr);
        a4->sin_family = AF_INET;
        a4->sin_port = htons(to.port);
        std::memcpy(&a4->sin_addr, to.ip.data(), 4);
        addrlen = sizeof(sockaddr_in);
    }
    int ret =
        ::sendto(handle, static_cast<const char *>(buffer), static_cast<int>(length), flags, reinterpret_cast<sockaddr *>(&addr), addrlen);
    return static_cast<ssize_t>(ret);
}
