#pragma once

#include <RTypeNet/Interfaces.hpp>
#include <thread>
#include <vector>

namespace rtype::srv {

[[nodiscard]] std::thread startTcpServer(const network::Endpoint &endpoint, std::atomic<bool> &quitServer) noexcept;

[[nodiscard]] std::vector<std::thread> startUdpServers(network::Endpoint baseEndpoint, std::size_t ncores,
    const network::Endpoint &tcpEndpoint, network::Endpoint externalUdpEndpoint, std::atomic<bool> &quitServer) noexcept;

}// namespace rtype::srv
