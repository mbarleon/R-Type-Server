#pragma once

#include <utility>

// clang-format off
#define RTYPE_NET_FWD_CORE_FUNC(API, PREFIX, NAME, QUALS, SUFFIX)                       \
namespace rtype::network {                                                              \
    API PREFIX inline decltype(auto) NAME(auto&&... args) QUALS                         \
        noexcept(noexcept(core::NAME(std::forward<decltype(args)>(args)...))) SUFFIX    \
    {                                                                                   \
        return core::NAME(std::forward<decltype(args)>(args)...);                       \
    }                                                                                   \
}
// clang-format on
