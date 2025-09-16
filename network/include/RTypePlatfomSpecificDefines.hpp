#pragma once

#include <cstring>

#if defined(_WIN32)
    #define RTYPE_AGNOSTIC_STRDUP_IMPL _strdup
#else
    #define RTYPE_AGNOSTIC_STRDUP_IMPL strdup
#endif
