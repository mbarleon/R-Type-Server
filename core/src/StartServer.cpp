#include "StartServer.hpp"
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Gateway.hpp>
#include <iostream>

std::thread rtype::srv::startTcpServer(const network::Endpoint &endpoint, std::atomic<bool> &quitServer) noexcept
{
    return std::thread([endpoint, &quitServer]() {
        Gateway &s = Gateway::getInstance();

        try {
            s.initServer(endpoint, quitServer);
            s.startServer();
        } catch (const Exception &e) {
            std::cerr << "Exception caught while running server: " << e.where() << ": " << e.what() << std::endl;
        }
    });
}

std::thread rtype::srv::startUdpServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
    std::atomic<bool> &quitServer) noexcept
{
    (void) baseEndpoint;
    (void) ncores;
    (void) tcpEndpoint;
    (void) quitServer;
    return std::thread();
}
