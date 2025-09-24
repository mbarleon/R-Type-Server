#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Server.hpp>

#if defined(_WIN32)
template class RTYPE_SRV_API rtype::srv::utils::Singleton<rtype::srv::Server>;
#endif

void rtype::srv::Server::stopServer()
{
    if (!_is_init) {
        return;
    }
}

void rtype::srv::Server::startServer()
{
    if (!_is_init) {
        throw Exception("startServer", "Server was not initialized.");
    }
}

void rtype::srv::Server::initServer(const network::Endpoint &tcp_endpoint, std::size_t n_cores)
{
    if (_is_init) {
        throw Exception("initServer", "Server was already initialized.");
    }
    _tcp_endpoint = tcp_endpoint;
    _n_cores = n_cores;
    _is_init = true;
}
