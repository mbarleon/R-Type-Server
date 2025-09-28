#pragma once

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include <RTypeNet/Interfaces.hpp>
#include <RTypeSrv/Api.hpp>
#include <RTypeSrv/Utils/Singleton.hpp>
#include <thread>
#include <unordered_map>

namespace rtype::srv {

class RTYPE_SRV_API Server final : public utils::Singleton<Server>
{
        friend class Singleton;

    public:
        void startServer();
        void initServer(const network::Endpoint &tcp_endpoint, std::atomic<bool> &quit_server);

    protected:
        explicit Server() = default;
        ~Server() noexcept = default;

    private:
        void _serverLoop();
        void _startServer();
        void _cleanupServer();

        bool _is_init = false;
        network::Socket _sock{};
        bool _is_running = false;
        network::Endpoint _tcp_endpoint{};
        std::atomic<bool> *_quit_server = nullptr;
        std::unordered_map<std::size_t, network::Socket> _sockets;
};

}// namespace rtype::srv

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
