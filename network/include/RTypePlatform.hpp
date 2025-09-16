#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
    #include "IOCPVersion.hpp"
#elif defined(__unix__) || defined(__APPLE__) || defined(__GNUC__)
    #include "POSIXVersion.hpp"
#else
    #error "Unsupported platform"
#endif
