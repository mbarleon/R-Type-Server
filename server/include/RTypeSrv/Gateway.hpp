#pragma once

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4251)
#endif

#include <RTypeNet/Interfaces.hpp>
#include <RTypeSrv/Api.hpp>
#include <RTypeSrv/Utils/Singleton.hpp>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <thread>
#include <unordered_map>
#include <vector>

namespace std {

template<>
struct hash<std::array<unsigned char, 16>> {
        std::size_t operator()(const std::array<unsigned char, 16> &arr) const noexcept
        {
            std::size_t h = 0;
            for (auto b : arr) {
                h = h * 31 + std::hash<unsigned char>{}(b);
            }
            return h;
        }
};

}// namespace std

namespace rtype::srv {

struct RTYPE_SRV_API pair_hash {
        std::size_t operator()(const std::pair<std::array<uint8_t, 16>, uint16_t> &p) const
        {
            const std::size_t h1 = std::hash<uint16_t>{}(p.second);
            std::size_t h2 = 0;
            for (const auto b : p.first)
                h2 ^= std::hash<uint8_t>{}(b) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
            return h1 ^ (h2 << 1);
        }
};

struct RTYPE_SRV_API array_hash {
        template<typename T, std::size_t N>
        std::size_t operator()(const std::array<T, N> &arr) const
        {
            std::size_t hash = 0;
            for (const auto &elem : arr) {
                hash ^= std::hash<T>{}(elem) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
};

class RTYPE_SRV_API Gateway final : public utils::Singleton<Gateway>
{
        friend class Singleton;

    public:
        void startServer();
        void initServer(const network::Endpoint &tcp_endpoint, std::atomic<bool> &quit_server);

    protected:
        explicit Gateway() = default;
        ~Gateway() noexcept = default;

    private:
        static constexpr int MAX_PARSE_ERRORS = 3;
        static constexpr size_t MAX_BUFFER_SIZE = 64 * 1024;
        static constexpr auto OCCUPANCY_INTERVAL = std::chrono::seconds(60);

        using clock = std::chrono::steady_clock;
        using IP = std::pair<std::array<uint8_t, 16>, uint16_t>;

        using FdsType = std::vector<network::PollFD>;
        using GameToGsType = std::unordered_map<uint32_t, IP>;
        using GsRegistryType = std::unordered_map<IP, int, pair_hash>;
        using OccupancyCacheType = std::unordered_map<IP, uint8_t, pair_hash>;
        using SocketsMapType = std::unordered_map<std::size_t, network::Socket>;
        using GsAddrToHandleType = std::unordered_map<IP, network::Handle, pair_hash>;
        using RecvSpanType = std::unordered_map<network::Handle, std::vector<uint8_t>>;
        using SendSpanType = std::unordered_map<network::Handle, std::vector<std::vector<uint8_t>>>;
        using PendingCreatesType = std::unordered_map<network::Handle, std::pair<network::Handle, uint8_t>>;

        void _serverLoop();
        void _startServer();
        void _cleanupServer();

        void _parsePackets();
        void sendOccupancyRequests();
        void _acceptClients() noexcept;
        void _recvPackets(network::NFDS i);
        void _sendPackets(network::NFDS i);
        void _handleLoop(network::NFDS &i) noexcept;
        void _handleClients(network::NFDS &i) noexcept;
        void _handleClientsSend(network::NFDS &i) noexcept;
        void _disconnectByHandle(const network::Handle &handle) noexcept;

        void setPolloutForHandle(network::Handle h) noexcept;
        void handleJoin(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize);
        void handleCreate(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize);
        void handleOccupancy(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize);
        static void handleOKKO(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize);
        void handleGSRegistration(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize);

        FdsType _fds;
        bool _is_init = false;
        network::NFDS _nfds = 1;
        network::Socket _sock{};
        SocketsMapType _sockets;
        SendSpanType _send_spans;
        RecvSpanType _recv_spans;
        std::size_t _next_id = 0;
        bool _is_running = false;
        GameToGsType _game_to_gs;
        GsRegistryType _gs_registry;
        network::Endpoint _tcp_endpoint{};
        PendingCreatesType _pending_creates;
        OccupancyCacheType _occupancy_cache;
        GsAddrToHandleType _gs_addr_to_handle;
        std::atomic<bool> *_quit_server = nullptr;
};

}// namespace rtype::srv

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
