#include <RTypeNet/Accept.hpp>
#include <system_error>
#include <winsock2.h>
#include <ws2tcpip.h>

RTYPE_NET_API rtype::network::Socket rtype::network::accept(Handle serverHandle)
{
    sockaddr_storage clientAddr{};
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket = ::accept(serverHandle, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);

    if (clientSocket == INVALID_SOCKET) {
        throw std::system_error(WSAGetLastError(), std::system_category(), "accept failed");
    }
    Socket result{};
    result.handle = clientSocket;
    if (clientAddr.ss_family == AF_INET) {
        auto *addr4 = reinterpret_cast<sockaddr_in *>(&clientAddr);
        result.endpoint.port = ntohs(addr4->sin_port);
        result.endpoint.ip.fill(0);
        std::memcpy(result.endpoint.ip.data(), &addr4->sin_addr, 4);
    } else if (clientAddr.ss_family == AF_INET6) {
        auto *addr6 = reinterpret_cast<sockaddr_in6 *>(&clientAddr);
        result.endpoint.port = ntohs(addr6->sin6_port);
        std::memcpy(result.endpoint.ip.data(), &addr6->sin6_addr, 16);
    }
    return result;
}
