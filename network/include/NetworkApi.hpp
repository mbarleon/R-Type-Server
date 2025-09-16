#pragma once

#if defined(_WIN32)
    #if defined(R_TYPE_NETWORK_BUILD)
        #define R_TYPE_NETWORK_API __declspec(dllexport)
    #else
        #define R_TYPE_NETWORK_API __declspec(dllimport)
    #endif
#else
    #define R_TYPE_NETWORK_API
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define R_TYPE_NETWORK_HIDE_FROM_ABI __attribute__((visibility("hidden")))
#else
    #define R_TYPE_NETWORK_HIDE_FROM_ABI
#endif
