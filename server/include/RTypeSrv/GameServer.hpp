#pragma once

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4251)
#endif

#include <RTypeNet/Interfaces.hpp>
#include <RTypeSrv/Api.hpp>
#include <RTypeSrv/Utils/NonCopyable.hpp>
#include <atomic>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace rtype::srv {

class RTYPE_SRV_API GameServer : public utils::NonCopyable
{
    public:
        GameServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
            std::atomic<bool> &quitServer);
        ~GameServer() noexcept = default;

        void StartServer() noexcept;

    private:
        using FdsType = std::vector<network::PollFD>;
        using SocketsMapType = std::unordered_map<std::size_t, network::Socket>;
        using RecvSpanType = std::unordered_map<network::Handle, std::vector<uint8_t>>;
        using SendSpanType = std::unordered_map<network::Handle, std::vector<std::vector<uint8_t>>>;

        void _initServer();
        void _serverLoop();
        void _cleanupServer();

        FdsType _fds{};
        network::NFDS _nfds = 1;
        SocketsMapType _sockets;
        network::Socket _sock{};
        std::size_t _ncores = 4;
        SendSpanType _send_spans;
        RecvSpanType _recv_spans;
        std::size_t _next_id = 0;
        bool _is_running = false;
        network::Handle _tcpHandle{};
        network::Socket _server_sock{};
        network::Endpoint _tcp_endpoint{};
        network::Endpoint _base_endpoint{};
        std::atomic<bool> *_quit_server = nullptr;
};

}// namespace rtype::srv

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
