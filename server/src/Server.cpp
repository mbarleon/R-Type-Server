#include "RTypeNet/Poll.hpp"

#include <RTypeNet/Cleanup.hpp>
#include <RTypeNet/Disconnect.hpp>
#include <RTypeNet/Listen.hpp>
#include <RTypeNet/Startup.hpp>
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Server.hpp>
#include <RTypeSrv/Utils/IPToStr.hpp>
#include <iostream>
#include <ranges>

#if defined(_WIN32)
template class RTYPE_SRV_API rtype::srv::utils::Singleton<rtype::srv::Server>;
#endif

void rtype::srv::Server::_startServer()
{
    if (!_is_init) {
        throw Exception("startServer", "Server was not initialized.");
    }
    if (_is_running) {
        throw Exception("startServer", "Server is already running.");
    }
    _is_running = true;
    network::startup();
    try {
        _sock = listen(_tcp_endpoint, network::Protocol::TCP);
    } catch (const std::exception &e) {
        throw Exception("startServer", "Could not start listening on ", utils::ipToStr(_tcp_endpoint.ip), ":", _tcp_endpoint.port, ": ",
            e.what());
    }
    std::cout << "TCP server listening on " << utils::ipToStr(_tcp_endpoint.ip) << ":" << _tcp_endpoint.port << "..." << std::endl;
}

void rtype::srv::Server::_serverLoop()
{
    while (!(*_quit_server)) {}
}

void rtype::srv::Server::_cleanupServer()
{
    for (auto &sock : _sockets | std::views::values) {
        disconnect(sock);
    }
    _sockets.clear();
    disconnect(_sock);
    _is_running = false;
    std::cout << "TCP server stopped." << std::endl;
    network::cleanup();
}

void rtype::srv::Server::startServer()
{
    _startServer();
    _serverLoop();
    _cleanupServer();
}

void rtype::srv::Server::initServer(const network::Endpoint &tcp_endpoint, std::atomic<bool> &quit_server)
{
    if (_is_init) {
        throw Exception("initServer", "Server was already initialized.");
    }
    _quit_server = &quit_server;
    _tcp_endpoint = tcp_endpoint;
    _is_init = true;
}
