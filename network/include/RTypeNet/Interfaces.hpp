#pragma once

#include <RTypeNet/Api.hpp>
#include <array>
#include <cstdint>
#include <span>

namespace rtype::network {

struct RTYPE_NET_API Endpoint {
        std::array<std::uint8_t, 16> ip{};
        std::uint16_t port{};
        bool operator==(const Endpoint &other) const noexcept = default;
};

struct RTYPE_NET_API RecvSpan {
        std::uint8_t *buf{};
        std::uint32_t len{};
        Endpoint from{};

        template<std::size_t n>
        constexpr explicit RecvSpan(std::uint8_t (&_buf)[n]) noexcept : buf(_buf), len(n)
        {
        }

        constexpr explicit RecvSpan(std::uint8_t *_buf, const std::uint32_t _len) noexcept : buf(_buf), len(_len)
        {
        }
};

struct RTYPE_NET_API SendSpan {
        const std::uint8_t *buf{};
        std::uint32_t len{};
        Endpoint to{};

        template<std::size_t n>
        constexpr explicit SendSpan(const std::uint8_t (&_buf)[n]) noexcept : buf(_buf), len(n)
        {
        }

        constexpr explicit SendSpan(const std::uint8_t *_buf, const std::uint32_t _len) noexcept : buf(_buf), len(_len)
        {
        }
};

struct RTYPE_NET_API PacketHeader {
        std::uint32_t matchId;
        std::uint16_t clientId;
        std::uint16_t seq;
        std::uint8_t chan;
        std::uint8_t flags;
}
#if defined(__GNUC__)
__attribute__((packed))
#endif
;

}// namespace rtype::network
