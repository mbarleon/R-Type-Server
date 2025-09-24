#pragma once

#include <RTypeNet/Interfaces.hpp>

namespace rtype::srv {

int parseArgs(int ac, const char *av[], rtype::network::Endpoint &endpoint, std::size_t &n_cores) noexcept;

}
