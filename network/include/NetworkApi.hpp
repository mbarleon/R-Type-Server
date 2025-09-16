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
