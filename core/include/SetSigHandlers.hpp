#pragma once
#include <cstddef>
#include <csignal>

namespace rtype::srv {

#ifdef _WIN32
void handle_signals(int sig);
#else
void handle_signals(int sig, [[maybe_unused]] siginfo_t *info, [[maybe_unused]] void *ucontext);
#endif

static constexpr int zap_srv_max_sig_count = 3;

void setSigHandlers() noexcept;

}// namespace rtype::srv
