#pragma once

#include <RTypeNet/Interfaces.hpp>
#include <string>

namespace rtype::srv {

struct Config {
        bool tcp_only = false;
        bool udp_only = false;
        network::Endpoint tcp_endpoint;
        network::Endpoint udp_endpoint;
        std::size_t n_cores = 4;
};

static constexpr uint16_t default_tcp_port = 3000;
static constexpr uint16_t default_udp_port = 4000;

Config getConfig(const std::string &filename);

}// namespace rtype::srv
