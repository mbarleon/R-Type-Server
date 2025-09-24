#include <RTypeNet/Subplatform/Listen.hpp>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>

RTYPE_NET_API rtype::network::Socket rtype::network::subplatform::listen(const Endpoint &e, Protocol p)
{
    int type = (p == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    int proto = (p == Protocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    int family = (e.ip[4] || e.ip[5] || e.ip[6] || e.ip[7] || e.ip[8]) ? AF_INET6 : AF_INET;
    int s = ::socket(family, type, proto);
    if (s == -1) {
        throw std::system_error(errno, std::system_category(), "socket creation failed");
    }
    sockaddr_storage addr{};
    socklen_t addrlen = 0;
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
    if (::bind(s, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1) {
        int err = errno;
        ::close(s);
        throw std::system_error(err, std::system_category(), "bind failed");
    }
    if (p == Protocol::TCP) {
        if (::listen(s, SOMAXCONN) == -1) {
            int err = errno;
            ::close(s);
            throw std::system_error(err, std::system_category(), "listen failed");
        }
    }
    Socket result{};
    result.handle = s;
    result.endpoint = e;
    return result;
}
