#include <RTypeNet/Disconnect.hpp>
#include <winsock2.h>

RTYPE_NET_API void rtype::network::disconnect(Socket &sock) noexcept
{
    if (sock.handle != INVALID_SOCK) {
        ::shutdown(sock.handle, SD_BOTH);
        ::closesocket(sock.handle);
        sock.handle = INVALID_SOCK;
        sock.endpoint.ip.fill(0);
        sock.endpoint.port = 0;
    }
}
