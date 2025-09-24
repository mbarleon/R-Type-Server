#include "ParseArgs.hpp"
#include "SetSigHandlers.hpp"
#include "StartServer.hpp"
#include <iostream>

static void printHelp() noexcept
{
    std::cout << "Usage: ./r-type_server -h host -p port -j n_cores" << std::endl;
}

int main(const int ac, const char *av[]) noexcept
{
    rtype::network::Endpoint endpoint;
    std::size_t n_cores;

    if (ac < 2) {
        printHelp();
        return 84;
    }
    if (const int ret = rtype::srv::parseArgs(ac, av, endpoint, n_cores); ret != 0) {
        printHelp();
        return ret == 84 ? 84 : 0;
    }
    rtype::srv::setSigHandlers();
    rtype::srv::startServer(endpoint, n_cores);
    return 0;
}
