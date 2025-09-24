#include <cstddef>

#if defined(_WIN32)
using ssize_t = SSIZE_T;
    #include <BaseTsd.h>
#else
    #include <sys/types.h>
#endif
