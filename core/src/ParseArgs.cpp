#include "ParseArgs.hpp"
#include <cstring>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif

static bool parseIPAddress(const std::string &host, rtype::network::Endpoint &endpoint) noexcept
{
    endpoint.ip.fill(0);

    struct sockaddr_in sa4;
    if (inet_pton(AF_INET, host.c_str(), &sa4.sin_addr) == 1) {
        std::memcpy(endpoint.ip.data(), &sa4.sin_addr, 4);
        return true;
    }

    struct sockaddr_in6 sa6;
    if (inet_pton(AF_INET6, host.c_str(), &sa6.sin6_addr) == 1) {
        std::memcpy(endpoint.ip.data(), &sa6.sin6_addr, 16);
        return true;
    }

    if (host == "localhost") {
        endpoint.ip[0] = 127;
        endpoint.ip[1] = 0;
        endpoint.ip[2] = 0;
        endpoint.ip[3] = 1;
        return true;
    }
    return false;
}

static bool parsePort(const std::string &portStr, rtype::network::Endpoint &endpoint) noexcept
{
    try {
        const int port = std::stoi(portStr);
        if (port < 1 || port > 65535) {
            return false;
        }
        endpoint.port = static_cast<std::uint16_t>(port);
        return true;
    } catch (...) {
        return false;
    }
}

static bool parseCoreCount(const std::string &coreStr, std::size_t &n_cores) noexcept
{
    try {
        const int cores = std::stoi(coreStr);
        if (cores < 1) {
            return false;
        }
        n_cores = static_cast<std::size_t>(cores);
        return true;
    } catch (...) {
        return false;
    }
}

int rtype::srv::parseArgs(int ac, const char *av[], rtype::network::Endpoint &endpoint, std::size_t &n_cores) noexcept
{
    bool hostSet = false;
    bool portSet = false;
    n_cores = 4;

    for (int i = 1; i < ac; ++i) {

        if (std::string arg = av[i]; arg == "--help") {
            return 1;
        } else if (arg == "-h" && i + 1 < ac) {
            if (std::string host = av[++i]; !parseIPAddress(host, endpoint)) {
                return 84;
            }
            hostSet = true;
        } else if (arg == "-p" && i + 1 < ac) {
            if (std::string portStr = av[++i]; !parsePort(portStr, endpoint)) {
                return 84;
            }
            portSet = true;
        } else if (arg == "-j" && i + 1 < ac) {
            if (std::string coreStr = av[++i]; !parseCoreCount(coreStr, n_cores)) {
                return 84;
            }
        } else {
            return 84;
        }
    }
    if (!hostSet || !portSet) {
        return 84;
    }
    return 0;
}
