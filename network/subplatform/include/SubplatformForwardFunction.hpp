#pragma once

// clang-format off

#define RTYPE_NET_FWD_FUNC(PREFIX, NAME, PARAMS, CALL_ARGS, SUFFIX)                 \
namespace rtype::network {                                                          \
    PREFIX inline auto NAME PARAMS noexcept(noexcept(subplatform::NAME CALL_ARGS))  \
        -> decltype(subplatform::NAME CALL_ARGS) SUFFIX                             \
    {                                                                               \
        return subplatform::NAME CALL_ARGS;                                         \
    }                                                                               \
}
// clang-format on
