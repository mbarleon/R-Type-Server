#include <RTypeSrv/Gateway.hpp>
#include <array>
#include <cstring>
#include <ranges>
#include <stdexcept>

void rtype::srv::Gateway::handleGSRegistration(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize)
{
    if (offset + 1 + 16 + 2 > bufsize) {
        throw std::runtime_error("Incomplete GS Registration packet");
    }

    std::array<uint8_t, 16> ip{};
    std::memcpy(ip.data(), data + offset + 1, 16);
    uint16_t port = static_cast<uint16_t>((data[offset + 17] << 8) | data[offset + 18]);
    std::pair<std::array<uint8_t, 16>, uint16_t> key = {ip, port};
    const bool already_registered = _gs_registry.count(key) > 0;
    _gs_registry[key] = 1;
    _gs_addr_to_handle[key] = handle;
    uint8_t response = already_registered ? 0 : 1;
    _send_spans[handle].push_back({response});
    offset += 1 + 16 + 2;
}

void rtype::srv::Gateway::handleCreate(network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize)
{
    if (offset + 2 > bufsize) {
        throw std::runtime_error("Incomplete CREATE packet");
    }

    uint8_t gametype = data[offset + 1];
    if (_gs_registry.empty()) {
        _send_spans[handle].push_back({0});
        offset += 2;
        return;
    }
    auto min_gs = _gs_registry.end();
    size_t min_occupancy = SIZE_MAX;
    for (auto it = _gs_registry.begin(); it != _gs_registry.end(); ++it) {
        size_t occ = 0;
        for (const auto &[game_id, gs_key] : _game_to_gs) {
            if (gs_key == it->first) {
                auto occ_it = _occupancy_cache.find(game_id);
                occ += (occ_it != _occupancy_cache.end()) ? occ_it->second : 0;
            }
        }
        if (occ < min_occupancy) {
            min_occupancy = occ;
            min_gs = it;
        }
    }
    if (min_gs == _gs_registry.end()) {
        _send_spans[handle].push_back({0});
        offset += 2;
        return;
    }
    auto &[gs_key, _] = *min_gs;
    network::Handle gs_handle = 0;
    if (const auto it_handle = _gs_addr_to_handle.find(gs_key); it_handle != _gs_addr_to_handle.end()) {
        gs_handle = it_handle->second;
    }
    if (gs_handle == 0) {
        _send_spans[handle].push_back({0});
        offset += 2;
        return;
    }
    std::vector create_msg = {3, gametype};
    _send_spans[gs_handle].push_back(std::move(create_msg));
    _pending_creates[gs_handle] = {handle, gametype};
    offset += 2;
}

void rtype::srv::Gateway::handleJoin(const network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize)
{
    if (offset + 1 + 4 > bufsize) {
        throw std::runtime_error("Incomplete JOIN packet");
    }

    const uint32_t id =
        static_cast<uint32_t>((data[offset + 1] << 24) | (data[offset + 2] << 16) | (data[offset + 3] << 8) | data[offset + 4]);
    if (_gs_registry.empty()) {
        _send_spans[handle].push_back({0});
    } else if (const auto it = _pending_creates.find(handle); it != _pending_creates.end()) {
        const network::Handle client_handle = it->second.first;
        std::vector join_msg(data + offset, data + offset + 1 + 16 + 2 + 4);
        _send_spans[client_handle].push_back(std::move(join_msg));
        _pending_creates.erase(it);
    } else if (_game_to_gs.contains(id)) {
        auto &[fst, snd] = _game_to_gs[id];
        std::vector<uint8_t> join_msg;
        join_msg.push_back(2);
        join_msg.insert(join_msg.end(), fst.begin(), fst.end());
        join_msg.push_back(snd >> 8);
        join_msg.push_back(snd & 0xFF);
        join_msg.push_back((id >> 24) & 0xFF);
        join_msg.push_back((id >> 16) & 0xFF);
        join_msg.push_back((id >> 8) & 0xFF);
        join_msg.push_back(id & 0xFF);
        _send_spans[handle].push_back(std::move(join_msg));
    } else {
        _send_spans[handle].push_back({0});
    }
    offset += 1 + 4;
}

void rtype::srv::Gateway::handleOccupancy([[maybe_unused]] network::Handle handle, const uint8_t *data, size_t &offset, size_t bufsize)
{
    if (offset + 1 + 4 + 1 > bufsize) {
        throw std::runtime_error("Incomplete OCCUPANCY packet");
    }

    const uint32_t id =
        static_cast<uint32_t>((data[offset + 1] << 24) | (data[offset + 2] << 16) | (data[offset + 3] << 8) | data[offset + 4]);
    const uint8_t occ = data[offset + 5];
    _occupancy_cache[id] = occ;
    offset += 1 + 4 + 1;
}

void rtype::srv::Gateway::handleOKKO([[maybe_unused]] network::Handle handle, [[maybe_unused]] const uint8_t *data, size_t &offset,
    size_t bufsize)
{
    if (offset + 1 > bufsize) {
        throw std::runtime_error("Incomplete OK/KO packet");
    }

    offset += 1;
}

void rtype::srv::Gateway::_parsePackets()
{
    std::unordered_map<network::Handle, int> parseErrors;

    for (auto &[handle, buf] : _recv_spans) {
        size_t offset = 0;
        while (offset < buf.size()) {
            try {
                switch (buf[offset]) {
                    case 20:
                        this->handleGSRegistration(handle, buf.data(), offset, buf.size());
                        break;
                    case 3:
                        this->handleCreate(handle, buf.data(), offset, buf.size());
                        break;
                    case 2:
                        this->handleJoin(handle, buf.data(), offset, buf.size());
                        break;
                    case 21:
                        this->handleOccupancy(handle, buf.data(), offset, buf.size());
                        break;
                    case 0:
                    case 1:
                        this->handleOKKO(handle, buf.data(), offset, buf.size());
                        break;
                    default:
                        throw std::runtime_error("Invalid packet sent by client.");
                }
            } catch (const std::exception &_) {
                parseErrors[handle]++;
                if (parseErrors[handle] >= MAX_PARSE_ERRORS) {
                    throw std::runtime_error("Client sent too many malformed packets.");
                }
                break;
            }
        }
        if (offset > 0 && offset <= buf.size()) {
            buf.erase(buf.begin(), buf.begin() + static_cast<long long>(offset));
        }
    }
}
