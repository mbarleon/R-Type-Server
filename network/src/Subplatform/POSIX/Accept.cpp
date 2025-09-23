#include <RTypeNet/Subplatform/Accept.hpp>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>

RTYPE_NET_API rtype::network::Socket rtype::network::subplatform::accept(Handle serverHandle)
{
    sockaddr_storage clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = ::accept(serverHandle, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);

    if (clientSocket == -1) {
        throw std::system_error(errno, std::system_category(), "accept failed");
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
