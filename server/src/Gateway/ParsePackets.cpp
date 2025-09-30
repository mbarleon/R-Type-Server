#include <RTypeSrv/Gateway.hpp>
#include <array>
#include <cstring>
#include <ranges>
#include <stdexcept>

void rtype::srv::Gateway::setPolloutForHandle(const network::Handle h) noexcept
{
    for (auto &fd : _fds) {
        if (fd.handle == h) {
            fd.events |= POLLOUT;
            break;
        }
    }
}

void rtype::srv::Gateway::handleGSRegistration(const network::Handle handle, const uint8_t *data, std::size_t &offset, std::size_t bufsize)
{
    if (offset + 1 + 16 + 2 > bufsize) {
        throw std::runtime_error("Incomplete GS Registration packet");
    }
    auto [ip, port] = parseGSKey(data, offset + 1);
    const std::pair key = {ip, port};
    const bool already_registered = _gs_registry.contains(key);
    _gs_registry[key] = 1;
    if (!already_registered) {
        _gs_addr_to_handle[key] = handle;
    }
    uint8_t response = already_registered ? 0 : 1;
    _send_spans[handle].push_back({response});
    setPolloutForHandle(handle);
    offset += 1 + 16 + 2;
}

void rtype::srv::Gateway::handleCreate(network::Handle handle, const uint8_t *data, std::size_t &offset, std::size_t bufsize)
{
    if (offset + 2 > bufsize) {
        throw std::runtime_error("Incomplete CREATE packet");
    }
    uint8_t gametype = data[offset + 1];
    if (_gs_registry.empty()) {
        sendErrorResponse(handle);
        offset += 2;
        return;
    }
    auto min_gs = findLeastOccupiedGS();
    if (!min_gs) {
        sendErrorResponse(handle);
        offset += 2;
        return;
    }
    auto &[gs_key, _] = **min_gs;
    const network::Handle gs_handle = getGSHandle(gs_key);
    if (gs_handle == 0) {
        sendErrorResponse(handle);
        offset += 2;
        return;
    }
    std::vector<uint8_t> create_msg = buildCreateMsg(gametype);
    _send_spans[gs_handle].push_back(std::move(create_msg));
    setPolloutForHandle(gs_handle);
    _pending_creates[gs_handle] = {handle, gametype};
    offset += 2;
}

std::pair<std::array<uint8_t, 16>, uint16_t> rtype::srv::Gateway::parseGSKey(const uint8_t *data, const std::size_t offset)
{
    std::array<uint8_t, 16> ip{};
    std::memcpy(ip.data(), data + offset, 16);
    uint16_t port = static_cast<uint16_t>((data[offset + 16] << 8) | data[offset + 17]);
    return {ip, port};
}

std::optional<rtype::srv::Gateway::GsRegistryType::iterator> rtype::srv::Gateway::findLeastOccupiedGS()
{
    auto min_gs = _gs_registry.end();
    std::size_t min_occupancy = SIZE_MAX;
    for (auto it = _gs_registry.begin(); it != _gs_registry.end(); ++it) {
        std::size_t occ = 0;
        auto occ_it = _occupancy_cache.find(it->first);
        occ = (occ_it != _occupancy_cache.end()) ? occ_it->second : 0;
        if (occ < min_occupancy) {
            min_occupancy = occ;
            min_gs = it;
        }
    }
    if (min_gs == _gs_registry.end()) {
        return std::nullopt;
    }
    return min_gs;
}

rtype::network::Handle rtype::srv::Gateway::getGSHandle(const IP &gs_key) const
{
    if (const auto it_handle = _gs_addr_to_handle.find(gs_key); it_handle != _gs_addr_to_handle.end()) {
        return it_handle->second;
    }
    return 0;
}

std::vector<uint8_t> rtype::srv::Gateway::buildCreateMsg(uint8_t gametype)
{
    return {3, gametype};
}

void rtype::srv::Gateway::sendErrorResponse(const network::Handle handle)
{
    _send_spans[handle].push_back({0});
    setPolloutForHandle(handle);
}

void rtype::srv::Gateway::handleJoin(const network::Handle handle, const uint8_t *data, std::size_t &offset, std::size_t bufsize)
{
    if (offset + 1 + 4 > bufsize) {
        throw std::runtime_error("Incomplete JOIN packet");
    }

    const uint32_t id = extractGameId(data + offset + 1);
    if (_gs_registry.empty()) {
        sendErrorResponse(handle);
    } else if (const auto it = _pending_creates.find(handle); it != _pending_creates.end()) {
        const network::Handle client_handle = it->second.first;
        std::vector<uint8_t> join_msg = buildJoinMsgForClient(data, offset);
        const uint32_t game_id = extractGameId(join_msg.data() + join_msg.size() - 4);
        if (const std::optional<IP> gs_key = findGSKeyByHandle(handle)) {
            _game_to_gs[game_id] = *gs_key;
        }
        _send_spans[client_handle].push_back(std::move(join_msg));
        setPolloutForHandle(client_handle);
        _pending_creates.erase(it);
    } else if (_game_to_gs.contains(id)) {
        auto &[fst, snd] = _game_to_gs[id];
        std::vector<uint8_t> join_msg = buildJoinMsgForGS(fst, snd, id);
        _send_spans[handle].push_back(std::move(join_msg));
        setPolloutForHandle(handle);
    } else {
        sendErrorResponse(handle);
    }
    offset += 1 + 4;
}

uint32_t rtype::srv::Gateway::extractGameId(const uint8_t *data) noexcept
{
    return (static_cast<uint32_t>(data[0]) << 24) | (static_cast<uint32_t>(data[1]) << 16) | (static_cast<uint32_t>(data[2]) << 8)
        | (static_cast<uint32_t>(data[3]));
}

std::optional<rtype::srv::Gateway::IP> rtype::srv::Gateway::findGSKeyByHandle(const network::Handle handle) const noexcept
{
    for (const auto &[key, h] : _gs_addr_to_handle) {
        if (h == handle) {
            return key;
        }
    }
    return std::nullopt;
}

std::vector<uint8_t> rtype::srv::Gateway::buildJoinMsgForClient(const uint8_t *data, std::size_t offset)
{
    return std::vector(data + offset, data + offset + 1 + 16 + 2 + 4);
}

std::vector<uint8_t> rtype::srv::Gateway::buildJoinMsgForGS(const std::array<uint8_t, 16> &ip, const uint16_t port, const uint32_t id)
{
    std::vector<uint8_t> join_msg;
    join_msg.push_back(2);
    join_msg.insert(join_msg.end(), ip.begin(), ip.end());
    join_msg.push_back(static_cast<uint8_t>(port >> 8));
    join_msg.push_back(static_cast<uint8_t>(port & 0xFF));
    join_msg.push_back(static_cast<uint8_t>((id >> 24) & 0xFF));
    join_msg.push_back(static_cast<uint8_t>((id >> 16) & 0xFF));
    join_msg.push_back(static_cast<uint8_t>((id >> 8) & 0xFF));
    join_msg.push_back(static_cast<uint8_t>(id & 0xFF));
    return join_msg;
}

void rtype::srv::Gateway::handleOccupancy([[maybe_unused]] network::Handle handle, const uint8_t *data, std::size_t &offset,
    std::size_t bufsize)
{
    if (offset + 1 + 16 + 2 + 1 > bufsize) {
        throw std::runtime_error("Incomplete OCCUPANCY packet");
    }
    auto [gs_key, occ] = parseOccupancy(data, offset + 1);
    _occupancy_cache[gs_key] = occ;
    offset += 1 + 16 + 2 + 1;
}

void rtype::srv::Gateway::handleOKKO([[maybe_unused]] network::Handle handle, [[maybe_unused]] const uint8_t *data, std::size_t &offset,
    std::size_t bufsize)
{
    if (offset + 1 > bufsize) {
        throw std::runtime_error("Incomplete OK/KO packet");
    }
    offset += 1;
}

void rtype::srv::Gateway::handleGID(const network::Handle handle, const uint8_t *data, std::size_t &offset, std::size_t bufsize)
{
    if (offset + 1 + 4 > bufsize) {
        throw std::runtime_error("Incomplete GID packet");
    }
    const std::size_t pos = offset + 1;
    const std::optional<IP> gs_key = findGSKeyByHandle(handle);
    if (!gs_key) {
        throw std::runtime_error("GS handle not registered");
    }
    const auto gids = parseGIDs(data, pos, bufsize);
    for (uint32_t gid : gids) {
        _game_to_gs[gid] = *gs_key;
    }
    offset = pos + gids.size() * 4;
}

std::pair<rtype::srv::Gateway::IP, uint8_t> rtype::srv::Gateway::parseOccupancy(const uint8_t *data, const std::size_t offset)
{
    std::array<uint8_t, 16> ip{};
    std::memcpy(ip.data(), data + offset, 16);
    uint16_t port = static_cast<uint16_t>((data[offset + 16] << 8) | data[offset + 17]);
    uint8_t occ = data[offset + 18];
    return {{ip, port}, occ};
}

std::vector<uint32_t> rtype::srv::Gateway::parseGIDs(const uint8_t *data, const std::size_t start, std::size_t bufsize)
{
    std::vector<uint32_t> gids;
    for (size_t pos = start; pos + 4 <= bufsize; pos += 4) {
        uint32_t gid = (static_cast<uint32_t>(data[pos]) << 24) | (static_cast<uint32_t>(data[pos + 1]) << 16)
            | (static_cast<uint32_t>(data[pos + 2]) << 8) | (static_cast<uint32_t>(data[pos + 3]));
        gids.push_back(gid);
    }
    return gids;
}

void rtype::srv::Gateway::_parsePackets()
{
    for (auto &[handle, buf] : _recv_spans) {
        std::size_t offset = 0;
        while (offset < buf.size()) {
            try {
                switch (buf[offset]) {
                    case 0:
                    case 1:
                        handleOKKO(handle, buf.data(), offset, buf.size());
                        break;
                    case 2:
                        handleJoin(handle, buf.data(), offset, buf.size());
                        break;
                    case 3:
                        handleCreate(handle, buf.data(), offset, buf.size());
                        break;
                    case 20:
                        handleGSRegistration(handle, buf.data(), offset, buf.size());
                        break;
                    case 21:
                        handleOccupancy(handle, buf.data(), offset, buf.size());
                        break;
                    case 22:
                        handleGID(handle, buf.data(), offset, buf.size());
                        break;
                    default:
                        throw std::runtime_error("Invalid packet sent by client.");
                }
            } catch (const std::exception &) {
                _parseErrors[handle]++;
                if (_parseErrors[handle] >= MAX_PARSE_ERRORS) {
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
