#pragma once

#include <utility>

// clang-format off
#define RTYPE_NET_FWD_FUNC(API, PREFIX, NAME, QUALS, SUFFIX)                                \
namespace rtype::network {                                                                  \
    API PREFIX inline decltype(auto) NAME(auto&&... args) QUALS                             \
        noexcept(noexcept(subplatform::NAME(std::forward<decltype(args)>(args)...))) SUFFIX \
    {                                                                                       \
        return subplatform::NAME(std::forward<decltype(args)>(args)...);                    \
    }                                                                                       \
}
// clang-format on
