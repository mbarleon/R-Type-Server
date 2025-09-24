#include <RTypeNet/Subplatform/Disconnect.hpp>
#include <sys/socket.h>
#include <unistd.h>

RTYPE_NET_API void rtype::network::subplatform::disconnect(Socket &sock) noexcept
{
    if (sock.handle != INVALID_SOCK) {
        ::shutdown(sock.handle, SHUT_RDWR);
        ::close(sock.handle);
        sock.handle = INVALID_SOCK;
        sock.endpoint.ip.fill(0);
        sock.endpoint.port = 0;
    }
}
