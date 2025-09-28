#include <RTypeNet/Send.hpp>
#include <RTypeSrv/Gateway.hpp>
#include <deque>
#include <utility>

namespace {

struct SendBuf {
        std::vector<uint8_t> data;
        size_t offset = 0;
};

std::deque<SendBuf> prepareSendQueue(std::vector<std::vector<uint8_t>> &bufs)
{
    std::deque<SendBuf> sendQueue;
    for (auto &buf : bufs) {
        if (!buf.empty())
            sendQueue.push_back(SendBuf{std::move(buf), 0});
    }
    bufs.clear();
    return sendQueue;
}

void processSendQueue(const rtype::network::Handle handle, std::deque<SendBuf> &sendQueue)
{
    while (!sendQueue.empty()) {
        auto &[data, offset] = sendQueue.front();
        const size_t to_send = data.size() - offset;
        const ssize_t sent = rtype::network::send(handle, data.data() + offset, to_send, 0);
        if (sent < 0) {
            break;
        }
        offset += static_cast<size_t>(sent);
        if (offset == data.size()) {
            sendQueue.pop_front();
        } else {
            break;
        }
    }
}

void updateSendBuffers(std::deque<SendBuf> &sendQueue, std::vector<std::vector<uint8_t>> &bufs)
{
    for (auto &[data, offset] : sendQueue) {
        if (offset < data.size()) {
            std::vector remaining(data.begin() + static_cast<long long>(offset), data.end());
            bufs.push_back(std::move(remaining));
        }
    }
}

}// namespace

void rtype::srv::Gateway::_sendPackets(const network::NFDS i)
{
    const auto handle = _fds[i].handle;
    if (!(_fds[i].revents & POLLOUT)) {
        return;
    }
    if (const auto it = _send_spans.find(handle); it != _send_spans.end()) {
        auto &bufs = it->second;
        auto sendQueue = prepareSendQueue(bufs);
        processSendQueue(handle, sendQueue);
        updateSendBuffers(sendQueue, bufs);
    }
}

void rtype::srv::Gateway::sendOccupancyRequests()
{
    for (const auto &[ip, port] : _gs_registry) {
        if (auto it = _gs_addr_to_handle.find({ip, port}); it != _gs_addr_to_handle.end()) {
            network::Handle gs_handle = it->second;
            std::vector<uint8_t> occ_req = {22};
            _send_spans[gs_handle].push_back(std::move(occ_req));
        }
    }
}
