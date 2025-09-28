#pragma once

#include <RTypeNet/Interfaces.hpp>
#include <thread>

namespace rtype::srv {

[[nodiscard]] std::thread startTcpServer(const network::Endpoint &endpoint, std::atomic<bool> &quitServer) noexcept;

[[nodiscard]] std::thread startUdpServer(const network::Endpoint &baseEndpoint, std::size_t ncores, const network::Endpoint &tcpEndpoint,
    std::atomic<bool> &quitServer) noexcept;

}// namespace rtype::srv
