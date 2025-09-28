#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace rtype::srv::utils {

std::string ipToStr(const std::array<uint8_t, 16> &ip) noexcept;

}
