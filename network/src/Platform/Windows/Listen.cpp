#include <RTypeNet/Listen.hpp>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>

RTYPE_NET_API rtype::network::Socket rtype::network::listen(const Endpoint &e, Protocol p)
{
    int type = (p == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    int proto = (p == Protocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    SOCKET s = ::socket((e.ip[0] || e.ip[1] || e.ip[2] || e.ip[3] || e.ip[4]) ? AF_INET6 : AF_INET, type, proto);
    if (s == INVALID_SOCKET) {
        throw std::system_error(WSAGetLastError(), std::system_category(), "socket creation failed");
    }
    sockaddr_storage addr{};
    int addrlen = 0;
    if (e.ip[0] || e.ip[1] || e.ip[2] || e.ip[3] || e.ip[4]) {
        sockaddr_in6 *a6 = reinterpret_cast<sockaddr_in6 *>(&addr);
        a6->sin6_family = AF_INET6;
        a6->sin6_port = htons(e.port);
        std::memcpy(&a6->sin6_addr, e.ip.data(), 16);
        addrlen = sizeof(sockaddr_in6);
    } else {
        sockaddr_in *a4 = reinterpret_cast<sockaddr_in *>(&addr);
        a4->sin_family = AF_INET;
        a4->sin_port = htons(e.port);
        std::memcpy(&a4->sin_addr, e.ip.data(), 4);
        addrlen = sizeof(sockaddr_in);
    }
    if (::bind(s, reinterpret_cast<sockaddr *>(&addr), addrlen) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        ::closesocket(s);
        throw std::system_error(err, std::system_category(), "bind failed");
    }
    if (p == Protocol::TCP) {
        if (::listen(s, SOMAXCONN) == SOCKET_ERROR) {
            int err = WSAGetLastError();
            ::closesocket(s);
            throw std::system_error(err, std::system_category(), "listen failed");
        }
    }
    Socket result{};
    result.handle = s;
    result.endpoint = e;
    return result;
}
