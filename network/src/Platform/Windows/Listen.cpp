#include <RTypeNet/Listen.hpp>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>

static SOCKET createSocket(rtype::network::Protocol p, int family)
{
    int type = (p == rtype::network::Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    int proto = (p == rtype::network::Protocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    SOCKET s = ::socket(family, type, proto);
    if (s == INVALID_SOCKET) {
        throw std::system_error(WSAGetLastError(), std::system_category(), "socket creation failed");
    }
    return s;
}

static void setSockOptions(SOCKET s)
{
    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&yes), sizeof(yes)) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        ::closesocket(s);
        throw std::system_error(err, std::system_category(), "setsockopt SO_REUSEADDR failed");
    }
}

static void bindSocket(SOCKET s, const rtype::network::Endpoint &e, int family)
{
    sockaddr_storage addr{};
    int addrlen = 0;

    if (family == AF_INET6) {
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
}

static void listenSocket(SOCKET s, rtype::network::Protocol p)
{
    if (p == rtype::network::Protocol::TCP) {
        if (::listen(s, SOMAXCONN) == SOCKET_ERROR) {
            int err = WSAGetLastError();
            ::closesocket(s);
            throw std::system_error(err, std::system_category(), "listen failed");
        }
    }
}

RTYPE_NET_API rtype::network::Socket rtype::network::listen(const Endpoint &e, Protocol p)
{
    int family = e.ip[0] || e.ip[1] || e.ip[2] || e.ip[3] || e.ip[4] ? AF_INET6 : AF_INET;
    SOCKET s = createSocket(p, family);

    setSockOptions(s);
    bindSocket(s, e, family);
    listenSocket(s, p);
    Socket result{};
    result.handle = s;
    result.endpoint = e;
    return result;
}
