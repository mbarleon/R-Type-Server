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

/**
 * @brief Constructs a new GameServer object.
 *
 * @param baseEndpoint The base endpoint of the server.
 * @param ncores The number of cores to use.
 * @param tcpEndpoint The TCP endpoint of the server.
 * @param quitServer A reference to an atomic boolean that will be set to true when the server should quit.
 */
rtype::srv::GameServer::GameServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
    std::atomic<bool> &quitServer)
{
    _ncores = ncores;
    _quit_server = &quitServer;
    _tcp_endpoint = tcpEndpoint;
    _base_endpoint = baseEndpoint;
}

/**
 * @brief Initializes the server.
 */
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

/**
 * @brief The main server loop.
 */
void rtype::srv::GameServer::_serverLoop()
{
    while (!(*_quit_server)) {}
}

/**
 * @brief Cleans up the server's resources.
 */
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

/**
 * @brief Starts the server.
 */
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
