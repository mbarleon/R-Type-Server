#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/GameServer.hpp>
#include <RTypeSrv/Utils/Logger.hpp>
#include <ranges>

rtype::srv::GameServer::GameServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
    std::atomic<bool> &quitServer)
{
    _ncores = ncores;
    _quit_server = &quitServer;
    _tcp_endpoint = tcpEndpoint;
    _base_endpoint = baseEndpoint;
}

void rtype::srv::GameServer::StartServer() noexcept
{
    if (_is_running) {
        return;
    }
    try {
        _initServer();
        _serverLoop();
        _cleanupServer();
    } catch (const Exception &e) {
        utils::cerr("Exception caught while running server: ", e.where(), ": ", e.what());
    }
}
