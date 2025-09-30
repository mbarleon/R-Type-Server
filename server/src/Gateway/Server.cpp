#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Gateway.hpp>
#include <RTypeSrv/Utils/Logger.hpp>

#if defined(_WIN32)
template class RTYPE_SRV_API rtype::srv::utils::Singleton<rtype::srv::Gateway>;
#endif

void rtype::srv::Gateway::startServer() noexcept
{
    try {
        _startServer();
        _serverLoop();
        _cleanupServer();
    } catch (const std::exception &e) {
        utils::cerr("Caught exception in server: ", e.what());
    }
}

void rtype::srv::Gateway::initServer(const network::Endpoint &tcp_endpoint, std::atomic<bool> &quit_server)
{
    if (_is_init) {
        throw Exception("initServer", "Server was already initialized.");
    }
    _quit_server = &quit_server;
    _tcp_endpoint = tcp_endpoint;
    _is_init = true;
}
