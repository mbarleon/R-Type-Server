#include "SetSigHandlers.hpp"
#include <RTypeSrv/Exception.hpp>
#include <RTypeSrv/Server.hpp>
#include <iostream>

#if !defined(_WIN32)
    #include <cstring>
    #include <sys/types.h>
#endif

#if defined(_WIN32)
void rtype::srv::handle_signals(int sig)
#else
void rtype::srv::handle_signals(int sig, [[maybe_unused]] siginfo_t *info, [[maybe_unused]] void *ucontext)
#endif
{
    static volatile sig_atomic_t sig_count = 0;

    if (sig == SIGINT) {
        std::cout << "\r" << std::flush;
    }
    if (sig_count == 0) {
        std::cout << "Stopping server..." << std::endl;
    }
    sig_count += 1;
    if (sig_count >= rtype::srv::zap_srv_max_sig_count) {
        std::cerr << "More than " << rtype::srv::zap_srv_max_sig_count << " SIGINTs detected... Force stopping server..." << std::endl;
        abort();
    }
    try {
        rtype::srv::Server::getInstance().stopServer();
    } catch (const rtype::srv::Exception &e) {
        std::cerr << "Caught exception while stopping server: " << e.where() << ": " << e.what() << std::endl;
    }
}

void rtype::srv::setSigHandlers() noexcept
{
#if defined(_WIN32)
    std::signal(SIGINT, handle_signals);
    std::signal(SIGTERM, handle_signals);
#else
    struct sigaction sa{};

    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handle_signals;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
#endif
}
