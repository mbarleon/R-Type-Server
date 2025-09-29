#include "StartServer.hpp"
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/GameServer.hpp>
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

std::vector<std::thread> rtype::srv::startUdpServers(network::Endpoint baseEndpoint, std::size_t ncores,
    const network::Endpoint &tcpEndpoint, std::atomic<bool> &quitServer) noexcept
{
    std::vector<std::thread> threads{};

    threads.reserve(ncores);
    for (std::size_t i = 0; i < ncores; ++i) {
        threads.emplace_back([baseEndpoint, ncores, tcpEndpoint, &quitServer]() {
            try {
                GameServer(baseEndpoint, ncores, tcpEndpoint, quitServer).StartServer();
            } catch (const Exception &e) {
                std::cerr << "Exception caught while running server: " << e.where() << ": " << e.what() << std::endl;
            }
        });
        ++baseEndpoint.port;
    }
    return threads;
}
