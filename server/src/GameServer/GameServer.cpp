#include <RTypeNet/Cleanup.hpp>
#include <RTypeNet/Connect.hpp>
#include <RTypeNet/Disconnect.hpp>
#include <RTypeNet/Listen.hpp>
#include <RTypeNet/Startup.hpp>
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/GameServer.hpp>
#include <RTypeSrv/Utils/IPToStr.hpp>
#include <RTypeSrv/Utils/Logger.hpp>
#include <iostream>
#include <ranges>

rtype::srv::GameServer::GameServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
    std::atomic<bool> &quitServer)
{
    _ncores = ncores;
    _quit_server = &quitServer;
    _tcp_endpoint = tcpEndpoint;
    _base_endpoint = baseEndpoint;
}

void rtype::srv::GameServer::_initServer()
{
    network::startup();
    try {
        _sock = listen(_base_endpoint, network::Protocol::UDP);
    } catch (const std::exception &e) {
        throw Exception("startServer", "Could not start listening on ", utils::ipToStr(_base_endpoint.ip), ":", _base_endpoint.port, ": ",
            e.what());
    }
    _fds.push_back({_sock.handle, POLLIN, 0});
    _is_running = true;
    _my_tcp_endpoint.ip = _base_endpoint.ip;
    utils::cout("Game server listening on ", utils::ipToStr(_base_endpoint.ip), ":", _base_endpoint.port, "...");
    _tcp_handle = network::connect(_my_tcp_endpoint, _tcp_endpoint, network::Protocol::TCP);
}

void rtype::srv::GameServer::_serverLoop()
{
    while (!(*_quit_server)) {}
}

void rtype::srv::GameServer::_cleanupServer()
{
    for (auto &sock : _sockets | std::views::values) {
        disconnect(sock);
    }
    _send_spans.clear();
    _recv_spans.clear();
    _sockets.clear();
    _fds.clear();
    _nfds = 0;
    _next_id = 0;
    disconnect(_sock);
    _is_running = false;
    utils::cout("Game server stopped.");
    network::cleanup();
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
