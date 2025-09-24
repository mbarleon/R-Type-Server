#pragma once

#include <RTypeNet/Interfaces.hpp>

namespace rtype::srv {

void startServer(const network::Endpoint &endpoint, std::size_t n_cores) noexcept;

}
