#include "GetConfig.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#if defined(_WIN32)
    #define _SSCANF sscanf_s
#else
    #define _SSCANF std::sscanf
#endif

static std::uint8_t fstPart(const int n) noexcept
{
    return static_cast<std::uint8_t>(n >> 8);
}

static std::uint8_t lastPart(const int n) noexcept
{
    return static_cast<std::uint8_t>(n & 0b0000000011111111);
}

static std::array<std::uint8_t, 16> buildIpV4(const int a, const int b, const int c, const int d) noexcept
{
    return {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, lastPart(a), lastPart(b), lastPart(c), lastPart(d)};
}

static std::array<std::uint8_t, 16> buildIpV6(const int a, const int b, const int c, const int d, const int e, const int f, const int g,
    const int h) noexcept
{
    return {fstPart(a), lastPart(a), fstPart(b), lastPart(b), fstPart(c), lastPart(c), fstPart(d), lastPart(d), fstPart(e), lastPart(e),
        fstPart(f), lastPart(f), fstPart(g), lastPart(g), fstPart(h), lastPart(h)};
}

static void getIp(const std::string &val, std::array<std::uint8_t, 16> &ip)
{
    int a, b, c, d, e, f, g, h;
    if (_SSCANF(val.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) == 4) {
        ip = buildIpV4(a, b, c, d);
    } else if (_SSCANF(val.c_str(), "%d:%d:%d:%d:%d:%d:%d:%d", &a, &b, &c, &d, &e, &f, &g, &h) == 8) {
        ip = buildIpV6(a, b, c, d, e, f, g, h);
    } else {
        throw std::invalid_argument("Invalid config file");
    }
}

static void getPort(const std::string &val, uint16_t &port)
{
    int p;

    if (_SSCANF(val.c_str(), "%d", &p) != 1 || p < 0 || p > 65535) {
        throw std::invalid_argument("Invalid config file");
    }
    port = static_cast<std::uint16_t>(p);
}

static void splitLine(const std::string &line, std::string &key, std::string &val)
{
    const std::size_t pos = line.find('=');

    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid config file");
    }
    key = line.substr(0, pos);
    val = line.substr(pos + 1);
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    val.erase(0, val.find_first_not_of(" \t"));
    val.erase(val.find_last_not_of(" \t") + 1);
}

static void checkEndpoint(rtype::network::Endpoint &endpoint, const std::array<std::uint8_t, 16> &default_ip, const uint16_t default_port)
{
    bool is_zero = true;

    for (const auto &byte : endpoint.ip) {
        if (byte != 0) {
            is_zero = false;
            break;
        }
    }
    if (is_zero) {
        endpoint.ip = default_ip;
    }
    if (endpoint.port == 0) {
        endpoint.port = default_port;
    }
}

static void checkEndpoint(rtype::network::Endpoint &endpoint, const uint16_t default_port)
{
    checkEndpoint(endpoint, buildIpV4(0, 0, 0, 0), default_port);
}

static void validateConfig(rtype::srv::Config &config)
{
    if (config.tcp_only && config.udp_only) {
        throw std::invalid_argument("Invalid config file");
    }
    if (config.tcp_only) {
        checkEndpoint(config.tcp_endpoint, rtype::srv::default_tcp_port);
    }
    if (config.udp_only) {
        checkEndpoint(config.tcp_endpoint, rtype::srv::default_tcp_port);
        checkEndpoint(config.udp_endpoint, rtype::srv::default_udp_port);
        checkEndpoint(config.external_udp_endpoint, config.udp_endpoint.ip, config.udp_endpoint.port);
        if (config.tcp_endpoint.port == config.udp_endpoint.port) {
            throw std::invalid_argument("Invalid config file");
        }
    }
}

rtype::srv::Config rtype::srv::getConfig(const std::string &filename)
{
    Config config;
    std::string line;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Config file does not exist");
    }
    while (std::getline(file, line)) {
        if (line[0] == '#' || line.empty() || !line.contains('=')) {
            continue;
        }
        std::string key, val;
        splitLine(line, key, val);
        if (key == "tcp_only") {
            config.tcp_only = (val == "true" || val == "1");
        } else if (key == "udp_only") {
            config.udp_only = (val == "true" || val == "1");
        } else if (key == "tcp_host") {
            getIp(val, config.tcp_endpoint.ip);
        } else if (key == "tcp_port") {
            getPort(val, config.tcp_endpoint.port);
        } else if (key == "udp_host") {
            getIp(val, config.udp_endpoint.ip);
        } else if (key == "udp_port") {
            getPort(val, config.udp_endpoint.port);
        } else if (key == "udp_external_host") {
            getIp(val, config.external_udp_endpoint.ip);
        } else if (key == "udp_external_port") {
            getPort(val, config.external_udp_endpoint.port);
        } else if (key == "n_cores") {
            std::size_t n_cores;
            if (_SSCANF(val.c_str(), "%zu", &n_cores) != 1 || n_cores == 0) {
                throw std::invalid_argument("Invalid config file");
            }
            config.n_cores = n_cores;
        }
    }
    validateConfig(config);
    return config;
}
