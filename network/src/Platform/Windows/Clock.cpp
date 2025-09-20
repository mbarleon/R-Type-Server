#include <RTypeNet/Clock.hpp>

#if defined(_WIN32)
    #include <windows.h>
#endif

RTYPE_NET_API std::uint64_t rtype::network::now_ns() noexcept
{
#if defined(_WIN32)
    LARGE_INTEGER freq, ctr;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&ctr);
    return static_cast<std::uint64_t>((ctr.QuadPart * 1000000000ULL) / freq.QuadPart);
#else
    return 0;
#endif
}
