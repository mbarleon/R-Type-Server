#include <RTypeNet/Accept.hpp>
#include <RTypeNet/Cleanup.hpp>
#include <RTypeNet/Disconnect.hpp>
#include <RTypeNet/Listen.hpp>
#include <RTypeNet/Poll.hpp>
#include <RTypeNet/Startup.hpp>
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Gateway.hpp>
#include <RTypeSrv/Utils/IPToStr.hpp>
#include <RTypeSrv/Utils/Logger.hpp>
#include <iostream>
#include <ranges>

void rtype::srv::Gateway::_startServer()
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
    _fds.push_back({_sock.handle, POLLIN, 0});
    utils::cout("TCP server listening on ", utils::ipToStr(_tcp_endpoint.ip), ":", _tcp_endpoint.port, "...");
}

void rtype::srv::Gateway::_handleClients(network::NFDS &i) noexcept
{
    try {
        _recvPackets(i);
        _parsePackets();
    } catch (const std::exception &e) {
        utils::cerr("Error handling client socket: ", e.what());
        _disconnectByHandle(_fds[i].handle);
        --i;
    }
}

void rtype::srv::Gateway::_handleClientsSend(network::NFDS &i) noexcept
{
    try {
        _sendPackets(i);
    } catch (const std::exception &e) {
        utils::cerr("Error handling client socket: ", e.what());
        _disconnectByHandle(_fds[i].handle);
        --i;
    }
}

void rtype::srv::Gateway::_handleLoop(network::NFDS &i) noexcept
{
    if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        _disconnectByHandle(_fds[i].handle);
        --i;
        return;
    }
    if (_fds[i].revents & POLLIN) {
        if (_fds[i].handle == _sock.handle) {
            _acceptClients();
        } else {
            _handleClients(i);
        }
    } else if (_fds[i].revents & POLLOUT && _fds[i].handle != _sock.handle) {
        _handleClientsSend(i);
    }
}

void rtype::srv::Gateway::_serverLoop()
{
    auto last_occupancy = clock::now();

    while (!(*_quit_server)) {
        if (network::poll(_fds.data(), _nfds, 0) == -1) {
            utils::cerr("Poll error, stopping server...");
            break;
        }
        for (network::NFDS i = 0; i < _nfds; ++i) {
            _handleLoop(i);
        }
        if (auto now = clock::now(); now - last_occupancy >= OCCUPANCY_INTERVAL) {
            sendOccupancyRequests();
            last_occupancy = now;
        }
    }
}

void rtype::srv::Gateway::_cleanupServer()
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
    utils::cout("TCP server stopped.");
    network::cleanup();
}
