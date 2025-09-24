#pragma once

#include <RTypeNet/Api.hpp>
#include <array>
#include <cstdint>
#include <span>

#if defined(_WIN32)
    #include <winsock2.h>
#else
    #include <poll.h>
#endif

namespace rtype::network {

#if defined(_WIN32)

typedef SOCKET Handle;
typedef unsigned long NFDS;
typedef int BufLen;
constexpr auto INVALID_SOCK = INVALID_SOCKET;

#else

typedef int Handle;
typedef nfds_t NFDS;
typedef size_t BufLen;
constexpr auto INVALID_SOCK = -1;

#endif

enum class Protocol : uint8_t { UDP, TCP };

struct RTYPE_NET_API PollFD {
        Handle handle;
        short events;
        short revents;
};

struct RTYPE_NET_API Endpoint {
        std::array<std::uint8_t, 16> ip{};
        std::uint16_t port{};
        bool operator==(const Endpoint &other) const noexcept = default;
};

struct RTYPE_NET_API Socket {
        Endpoint endpoint{};
        Handle handle{};
        Protocol protocol{};
        bool operator==(const Socket &other) const noexcept = default;
};

}// namespace rtype::network
