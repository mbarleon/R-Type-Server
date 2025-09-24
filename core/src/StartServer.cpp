#include "StartServer.hpp"
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Server.hpp>
#include <iostream>

void rtype::srv::startServer(const network::Endpoint &endpoint, const std::size_t n_cores) noexcept
{
    Server &s = Server::getInstance();

    try {
        s.initServer(endpoint, n_cores);
        s.startServer();
    } catch (const Exception &e) {
        try {
            s.stopServer();
        } catch (const Exception &e2) {
            std::cerr << "Exception caught while stopping server after exception in startServer(): " << e2.where() << ": " << e2.what()
                      << std::endl;
        }
        std::cerr << "Exception caught while running server: " << e.where() << ": " << e.what() << std::endl;
    }
}
